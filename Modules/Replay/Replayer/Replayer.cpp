/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Replayer.h"

namespace IsoRealms::Replay {
  Replayer::Replayer(Replay& replay, IResourceData& data) :
            cResource(data),
            cRuntimeState(State::INACTIVE),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }
  
  void Replayer::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, "", "Replayer");
    for (std::unique_ptr<DigitalInput>& mInput : cDefDigitalInputs) {
      mInput->registerAssets(assets);
    }
    for (std::unique_ptr<AnalogueInput>& mInput : cDefAnalogueInputs) {
      mInput->registerAssets(assets);
    }
  }
  
  bool Replayer::renderIcon() const {
    return false;
  }
  
  void Replayer::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void Replayer::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyArray("digitalInputs", cDefDigitalInputs, [](const std::unique_ptr<DigitalInput>& i)->DigitalInput& {return *i;}, [this, &owner, &metadata](DigitalInput& digitalInput) {
      owner.createPropertyStruct("DigitalInput", digitalInput.getName(), [&metadata, &digitalInput](IPropertyMaker& owner) {
        digitalInput.getProperties(owner, metadata);
      }, [this, &digitalInput]() {
        Utils::removeElementUnique(cDefDigitalInputs, &digitalInput);
        // TODO: Adjust ID's.
      });
    }, [this]()->DigitalInput& {
      return *cDefDigitalInputs.emplace_back(std::make_unique<DigitalInput>(*this, cResource));
      // TODO: Adjust ID's.
    });
    owner.createPropertyArray("analogueInputs", cDefAnalogueInputs, [](const std::unique_ptr<AnalogueInput>& i)->AnalogueInput& {return *i;}, [this, &owner, &metadata](AnalogueInput& analogueInput) {
      owner.createPropertyStruct("AnalogueInput", analogueInput.getName(), [&metadata, &analogueInput](IPropertyMaker& owner) {
        analogueInput.getProperties(owner, metadata);
      }, [this, &analogueInput]() {
        Utils::removeElementUnique(cDefAnalogueInputs, &analogueInput);
        // TODO: Adjust ID's.
      });
    }, [this]()->AnalogueInput& {
      return *cDefAnalogueInputs.emplace_back(std::make_unique<AnalogueInput>(*this, cResource));
      // TODO: Adjust ID's.
    });
  }

  void Replayer::removed() {
    // Nothing to do.
  }
  
  void Replayer::updateInputs(unsigned int milliseconds) {
    switch (cRuntimeState) {
      case State::RECORDING: {
        // Nothing to do.
        break;
      }
      
      case State::REPLAYING: {
        while (cRuntimeNextEvent.cTime == cRuntimeElapsedTime) {
          if (cRuntimeNextEvent.cID < cDefDigitalInputs.size()) {
            cDefDigitalInputs[cRuntimeNextEvent.cID]->setRecordedState(cRuntimeNextEvent.cState.cDigital);
          } else {
            cDefAnalogueInputs[cRuntimeNextEvent.cID - cDefDigitalInputs.size()]->setRecordedState(cRuntimeNextEvent.cState.cAnalogue);
          }
          readEvent();
        }
        cRuntimeElapsedTime += milliseconds;
        break;
      }
      
      case State::INACTIVE: {
        // Nothing to do.
        break;
      }
    }
  }
  
  void Replayer::updateRuntime(unsigned int milliseconds) {
    switch (cRuntimeState) {
      case State::RECORDING: {
        cRuntimeElapsedTime += milliseconds;
        break;
      }
      
      case State::REPLAYING: {
        // Nothing to do.
        break;
      }
      
      case State::INACTIVE: {
        // Nothing to do.
        break;
      }
    }
  }
  
  void Replayer::reset() {
    cRuntimeState = State::INACTIVE;
    cRuntimeElapsedTime = 0;
    if (cRuntimeRecordingOutput.is_open()) {
      cRuntimeRecordingOutput.close();
    }
    if (cRuntimeRecordedInput.is_open()) {
      cRuntimeRecordedInput.close();
    }
    cRuntimeNextEvent.cID = 0;
    cRuntimeNextEvent.cState.cAnalogue = 0.0f;
    cRuntimeNextEvent.cTime = 0;

    // Reset all input states.
    for (std::unique_ptr<DigitalInput>& mInput : cDefDigitalInputs) {
      mInput->reset();
    }
    for (std::unique_ptr<AnalogueInput>& mInput : cDefAnalogueInputs) {
      mInput->reset();
    }
  }
  
  void Replayer::setRecording(const std::string& file) {
    cRuntimeState = State::RECORDING;
    Project& mProject = cResource.getProject();
    mProject.makeUserDataDirectory();
    std::string mFilename = mProject.getDataPath(true) + "/" + file;
    if (cRuntimeRecordingOutput.is_open()) {
      cRuntimeRecordingOutput.close();
    }
    cRuntimeRecordingOutput.open(mFilename, std::ios::out | std::ios::binary);
  }

  void Replayer::setReplaying(const std::string& file, bool user) {
    cRuntimeState = State::REPLAYING;
    Project& mProject = cResource.getProject();
    std::string mFilename = mProject.getDataPath(user) + "/" + file;
    cRuntimeRecordedInput = std::ifstream(mFilename, std::ios::binary);

    // Read first event.
    readEvent();
  }

  void Replayer::setInactive() {
    cRuntimeState = State::INACTIVE;
  }

  Replayer::DigitalInput::DigitalInput(Replayer& parent, IResourceData& data) :
            cParent(parent),
            cDefActualInput(data, false, [this](bool value) {
              if (cParent.cRuntimeState == State::RECORDING) {
                cParent.writeEvent(cRuntimeID, value);
              }
              cStateNotifier->stateChanged();
            }),
            cRuntimeID(cParent.cDefDigitalInputs.size() + cParent.cDefAnalogueInputs.size()),
            cRuntimeRecordedInput(false) {
  }
  
  Replayer::DigitalInput::DigitalInput(Replayer& parent, IResourceData& data, JSONObject object) :
            cParent(parent),
            cDefName(object.getString(JSON_NAME)),
            cDefActualInput(data, false, [this](bool value) {
              if (cParent.cRuntimeState == State::RECORDING) {
                cParent.writeEvent(cRuntimeID, value);
              }
              cStateNotifier->stateChanged();
            }),
            cRuntimeID(cParent.cDefDigitalInputs.size() + cParent.cDefAnalogueInputs.size()),
            cRuntimeRecordedInput(false) {
    cDefActualInput.init(object, JSON_VALUE);
  }
  
  void Replayer::DigitalInput::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IBoolean>(this, cDefName, "Replayer Digital Input");
  }

  void Replayer::DigitalInput::save(JSONObject object) const {
    object.addString(JSON_NAME, cDefName);
    cDefActualInput.save(object, JSON_VALUE);
  }
  
  void Replayer::DigitalInput::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString(JSON_NAME,  [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, "", [this](const std::string& value) {return cParent.isInputNameAllowed(*this, value);});
    owner.createPropertyTreeSelector(JSON_VALUE, cDefActualInput);
  }
  
  void Replayer::DigitalInput::reset() {
    cRuntimeRecordedInput = false;
  }
  
  void Replayer::DigitalInput::setRecordedState(bool state) {
    if (cRuntimeRecordedInput != state) {
      cRuntimeRecordedInput = state;
      cStateNotifier->stateChanged();
    }
  }
  
  std::string Replayer::DigitalInput::getName() const {
    return cDefName;
  }

  bool Replayer::DigitalInput::getValue() const {
    return cParent.cRuntimeState == State::REPLAYING ? cRuntimeRecordedInput : cDefActualInput->getValue();
  }
  
  bool Replayer::DigitalInput::renderAssetIcon() const {
    return false;
  }
  
  void Replayer::DigitalInput::saveAsset(JSONObject object) const {
    // TODO: Implement this.
  }
  
  void Replayer::DigitalInput::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }
  
  bool Replayer::DigitalInput::isDefaultConfiguration() const {
    return true;
  }
  
  Replayer::AnalogueInput::AnalogueInput(Replayer& parent, IResourceData& data) :
            cParent(parent),
            cDefActualInput(data, 0.0f, [this](float value) {
              if (cParent.cRuntimeState == State::RECORDING) {
                cParent.writeEvent(cRuntimeID, value);
              }
              cStateNotifier->stateChanged();
            }),
            cRuntimeID(cParent.cDefDigitalInputs.size() + cParent.cDefAnalogueInputs.size()),
            cRuntimeRecordedInput(0.0f) {
  }
  
  Replayer::AnalogueInput::AnalogueInput(Replayer& parent, IResourceData& data, JSONObject object) :
            cParent(parent),
            cDefName(object.getString(JSON_NAME)),
            cDefActualInput(data, 0.0f, [this](float value) {
              if (cParent.cRuntimeState == State::RECORDING) {
                cParent.writeEvent(cRuntimeID, value);
              }
              cStateNotifier->stateChanged();
            }),
            cRuntimeID(cParent.cDefDigitalInputs.size() + cParent.cDefAnalogueInputs.size()),
            cRuntimeRecordedInput(0.0f) {
    cDefActualInput.init(object, JSON_VALUE);
  }
  
  void Replayer::AnalogueInput::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IFloat>(this, cDefName, "Replayer Analogue Input");
  }

  void Replayer::AnalogueInput::save(JSONObject object) const {
    object.addString(JSON_NAME, cDefName);
    cDefActualInput.save(object, JSON_VALUE);
  }
  
  void Replayer::AnalogueInput::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString(JSON_NAME,  [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, "", [this](const std::string& value) {return cParent.isInputNameAllowed(*this, value);});
    owner.createPropertyTreeSelector(JSON_VALUE, cDefActualInput);
  }
      
  void Replayer::AnalogueInput::reset() {
    cRuntimeRecordedInput = 0.0f;
  }
  
  void Replayer::AnalogueInput::setRecordedState(float state) {
    if (cRuntimeRecordedInput != state) {
      cRuntimeRecordedInput = state;
      cStateNotifier->stateChanged();
    }
  }
  
  std::string Replayer::AnalogueInput::getName() const {
    return cDefName;
  }

  float Replayer::AnalogueInput::getValue() const {
    return cParent.cRuntimeState == State::REPLAYING ? cRuntimeRecordedInput : cDefActualInput->getValue();
  }
  
  bool Replayer::AnalogueInput::renderAssetIcon() const {
    return false;
  }
  
  void Replayer::AnalogueInput::saveAsset(JSONObject object) const {
    // TODO: Implement this.
  }
  
  void Replayer::AnalogueInput::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }
  
  bool Replayer::AnalogueInput::isDefaultConfiguration() const {
    return true;
  }
  
  void Replayer::readEvent() {
    if (cRuntimeRecordedInput.eof()) {
      // TODO: Action on end of recording???
      return;
    }

    cRuntimeRecordedInput.read(reinterpret_cast<char*>(&cRuntimeNextEvent.cID), sizeof(cRuntimeNextEvent.cID));
    if (cRuntimeNextEvent.cID < cDefDigitalInputs.size()) {
      cRuntimeRecordedInput.read(reinterpret_cast<char*>(&cRuntimeNextEvent.cState.cDigital), sizeof(cRuntimeNextEvent.cState.cDigital));
    } else {
      cRuntimeRecordedInput.read(reinterpret_cast<char*>(&cRuntimeNextEvent.cState.cAnalogue), sizeof(cRuntimeNextEvent.cState.cAnalogue));
    }
    cRuntimeRecordedInput.read(reinterpret_cast<char*>(&cRuntimeNextEvent.cTime), sizeof(cRuntimeNextEvent.cTime));
  }
  
  void Replayer::writeEvent(unsigned int id, bool state) {
    cRuntimeRecordingOutput.write(reinterpret_cast<const char*>(&id),                  sizeof(id));
    cRuntimeRecordingOutput.write(reinterpret_cast<const char*>(&state),               sizeof(state));
    cRuntimeRecordingOutput.write(reinterpret_cast<const char*>(&cRuntimeElapsedTime), sizeof(cRuntimeElapsedTime));
    cRuntimeRecordingOutput.flush();
  }
  
  void Replayer::writeEvent(unsigned int id, float state) {
    cRuntimeRecordingOutput.write(reinterpret_cast<const char*>(&id),                  sizeof(id));
    cRuntimeRecordingOutput.write(reinterpret_cast<const char*>(&state),               sizeof(state));
    cRuntimeRecordingOutput.write(reinterpret_cast<const char*>(&cRuntimeElapsedTime), sizeof(cRuntimeElapsedTime));
    cRuntimeRecordingOutput.flush();
  }
  
  bool Replayer::isInputNameAllowed(DigitalInput& input, const std::string& name) const {
    for (const std::unique_ptr<DigitalInput>& mInput : cDefDigitalInputs) {
      if (mInput->getName() == name && mInput.get() != &input) {
        return false;
      }
    }
    for (const std::unique_ptr<AnalogueInput>& mInput : cDefAnalogueInputs) {
      if (mInput->getName() == name) {
        return false;
      }
    }
    return true;
  }
  
  bool Replayer::isInputNameAllowed(AnalogueInput& input, const std::string& name) const {
    for (const std::unique_ptr<DigitalInput>& mInput : cDefDigitalInputs) {
      if (mInput->getName() == name) {
        return false;
      }
    }
    for (const std::unique_ptr<AnalogueInput>& mInput : cDefAnalogueInputs) {
      if (mInput->getName() == name && mInput.get() != &input) {
        return false;
      }
    }
    return true;
  }
}
