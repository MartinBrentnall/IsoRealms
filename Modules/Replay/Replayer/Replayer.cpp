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
            cState(State::INACTIVE) {
  }
  
  Replayer::Replayer(Replay& replay, IResourceData& data, JSONObject object) :
            Replayer(replay, data) {

    // Read configuration.
    for (JSONValue mDigitalInputValue : object.getArray(JSON_DIGITAL_INPUTS)) {
      cDigitalInputs.emplace_back(std::make_unique<DigitalInput>(*this, data, mDigitalInputValue.getObject()));
    }
    for (JSONValue mAnalogueInputValue : object.getArray(JSON_ANALOGUE_INPUTS)) {
      cAnalogueInputs.emplace_back(std::make_unique<AnalogueInput>(*this, data, mAnalogueInputValue.getObject()));
    }
  }

  void Replayer::registerAssets(ResourceAssetRegistry& assets) {
    for (std::unique_ptr<DigitalInput>& mInput : cDigitalInputs) {
      mInput->registerAssets(assets);
    }
    for (std::unique_ptr<AnalogueInput>& mInput : cAnalogueInputs) {
      mInput->registerAssets(assets);
    }
  }
  
  void Replayer::save(JSONObject object) const {
    JSONArray mDigitalInputsArray = object.addArray(JSON_DIGITAL_INPUTS);
    for (const std::unique_ptr<DigitalInput>& mInput : cDigitalInputs) {
      JSONObject mDigitalInputObject = mDigitalInputsArray.addObject();
      mInput->save(mDigitalInputObject);
    }
    JSONArray mAnalogueInputsArray = object.addArray(JSON_ANALOGUE_INPUTS);
    for (const std::unique_ptr<AnalogueInput>& mInput : cAnalogueInputs) {
      JSONObject mAnalogueInputObject = mAnalogueInputsArray.addObject();
      mInput->save(mAnalogueInputObject);
    }
  }
  
  bool Replayer::renderIcon() const {
    return false;
  }
  
  void Replayer::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void Replayer::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyArray(metadata.getPropertyData("DigitalInputAdd"), cDigitalInputs, [](const std::unique_ptr<DigitalInput>& i)->DigitalInput& {return *i;}, [this, &owner, &metadata](DigitalInput& digitalInput) {
      owner.createPropertyStruct(metadata.getPropertyData("DigitalInput"), digitalInput.getName(), [&metadata, &digitalInput](PropertyMaker& owner) {
        digitalInput.getProperties(owner, metadata);
      }, [this, &digitalInput]() {
        Utils::removeElementUnique(cDigitalInputs, &digitalInput);
        // TODO: Adjust ID's.
      });
    }, [this]()->DigitalInput& {
      return *cDigitalInputs.emplace_back(std::make_unique<DigitalInput>(*this, cResource));
      // TODO: Adjust ID's.
    });
    owner.createPropertyArray(metadata.getPropertyData("AnalogueInputAdd"), cAnalogueInputs, [](const std::unique_ptr<AnalogueInput>& i)->AnalogueInput& {return *i;}, [this, &owner, &metadata](AnalogueInput& analogueInput) {
      owner.createPropertyStruct(metadata.getPropertyData("AnalogueInput"), analogueInput.getName(), [&metadata, &analogueInput](PropertyMaker& owner) {
        analogueInput.getProperties(owner, metadata);
      }, [this, &analogueInput]() {
        Utils::removeElementUnique(cAnalogueInputs, &analogueInput);
        // TODO: Adjust ID's.
      });
    }, [this]()->AnalogueInput& {
      return *cAnalogueInputs.emplace_back(std::make_unique<AnalogueInput>(*this, cResource));
      // TODO: Adjust ID's.
    });
  }
  
  void Replayer::updateInputs(unsigned int milliseconds) {
    switch (cState) {
      case State::RECORDING: {
        // Nothing to do.
        break;
      }
      
      case State::REPLAYING: {
        while (cNextEvent.cTime == cElapsedTime) {
          if (cNextEvent.cID < cDigitalInputs.size()) {
            cDigitalInputs[cNextEvent.cID]->setRecordedState(cNextEvent.cState.cDigital);
          } else {
            cAnalogueInputs[cNextEvent.cID - cDigitalInputs.size()]->setRecordedState(cNextEvent.cState.cAnalogue);
          }
          readEvent();
        }
        cElapsedTime += milliseconds;
        break;
      }
      
      case State::INACTIVE: {
        // Nothing to do.
        break;
      }
    }
  }
  
  void Replayer::updateRuntime(unsigned int milliseconds) {
    switch (cState) {
      case State::RECORDING: {
        if (cElapsedTime == 0) {

          // Construct date and time string for filename
          time_t mCurrentTime = time(0);
          struct tm* mNow = localtime(&mCurrentTime);
          std::stringstream mDateTimeReader;
          mDateTimeReader << std::setfill('0') << std::setw(2) << (mNow->tm_year - 100)
                                               << std::setw(2) << (mNow->tm_mon + 1)
                                               << std::setw(2) <<  mNow->tm_mday << "-"
                                               << std::setw(2) <<  mNow->tm_hour
                                               << std::setw(2) <<  mNow->tm_min
                                               << std::setw(2) <<  mNow->tm_sec;
          cFilenameString.setValue("Recordings/" + mDateTimeReader.str());
          cOutput = System::openOutputStream(cFilenameString.getValue());
        }
        cElapsedTime += milliseconds;
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
    cElapsedTime = 0;
    switch (cState) {
      case State::RECORDING: {
        if (cOutput.is_open()) {
          cOutput.close();
        }
        break;
      }
      
      case State::REPLAYING: {
        cRecording   = std::ifstream(cFilename, std::ios::binary);

        // Obtain project file.
        std::string mProjectFile;
        size_t mLength;
        cRecording.read(reinterpret_cast<char*>(&mLength), sizeof(mLength));
        mProjectFile.resize(mLength);
        cRecording.read(&mProjectFile[0], mLength);

        // Obtain configuration file.
        std::string mConfigurationFile;
        cRecording.read(reinterpret_cast<char*>(&mLength), sizeof(mLength));
        mConfigurationFile.resize(mLength);
        cRecording.read(&mConfigurationFile[0], mLength);

        // Reset inputs.
        for (std::unique_ptr<DigitalInput>& mInput : cDigitalInputs) {
          mInput->setRecordedState(false);
        }
        for (std::unique_ptr<AnalogueInput>& mInput : cAnalogueInputs) {
          mInput->setRecordedState(0.0f);
        }

        // Read first event.
        readEvent();
        break;
      }
      
      case State::INACTIVE: {
        // Nothing to do.
        break;
      }
    }
  }
  
  void Replayer::setRecording() {
    cState = State::RECORDING;
  }

  void Replayer::setReplaying(const std::string& file, bool user) {
    cState = State::REPLAYING;
    cFilename = System::getPath(file, user);
    cRecording = std::ifstream(cFilename, std::ios::binary);
  }

  void Replayer::setInactive() {
    cState = State::INACTIVE;
  }

  Replayer::DigitalInput::DigitalInput(Replayer& parent, IResourceData& data) :
            cParent(parent),
            cDefActualInput(data, false, [this](bool value) {
              if (cParent.cState == State::RECORDING) {
                cParent.writeEvent(cRuntimeID, value);
              }
            }),
            cRuntimeID(cParent.cDigitalInputs.size() + cParent.cAnalogueInputs.size()),
            cRuntimeRecordedInput(false) {
  }
  
  Replayer::DigitalInput::DigitalInput(Replayer& parent, IResourceData& data, JSONObject object) :
            cParent(parent),
            cDefName(object.getString(JSON_NAME)),
            cDefActualInput(data, false, [this](bool value) {
              if (cParent.cState == State::RECORDING) {
                cParent.writeEvent(cRuntimeID, value);
              }
              cStateNotifier->stateChanged(this);
            }),
            cRuntimeID(cParent.cDigitalInputs.size() + cParent.cAnalogueInputs.size()),
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
  
  void Replayer::DigitalInput::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString(  metadata.getPropertyData("DigitalInputName"),  [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, [this](const std::string& value) {return cParent.isInputNameAllowed(*this, value);});
    owner.createPropertyAsset<Boolean>(metadata.getPropertyData("DigitalInputValue"), cDefActualInput);
  }
  
  void Replayer::DigitalInput::setRecordedState(bool state) {
    cRuntimeRecordedInput = state;
  }
  
  std::string Replayer::DigitalInput::getName() const {
    return cDefName;
  }

  bool Replayer::DigitalInput::getValue() const {
    return cParent.cState == State::REPLAYING ? cRuntimeRecordedInput : cDefActualInput->getValue();
  }
  
  bool Replayer::DigitalInput::renderAssetIcon() const {
    return false;
  }
  
  void Replayer::DigitalInput::saveAsset(JSONObject object) const {
    // TODO: Implement this.
  }
  
  void Replayer::DigitalInput::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }
  
  bool Replayer::DigitalInput::isDefaultConfiguration() const {
    return true;
  }
  
  Replayer::AnalogueInput::AnalogueInput(Replayer& parent, IResourceData& data) :
            cParent(parent),
            cDefActualInput(data, 0.0f, [this](float value) {
              if (cParent.cState == State::RECORDING) {
                cParent.writeEvent(cRuntimeID, value);
              }
            }),
            cRuntimeID(cParent.cDigitalInputs.size() + cParent.cAnalogueInputs.size()),
            cRuntimeRecordedInput(0.0f) {
  }
  
  Replayer::AnalogueInput::AnalogueInput(Replayer& parent, IResourceData& data, JSONObject object) :
            cParent(parent),
            cDefName(object.getString(JSON_NAME)),
            cDefActualInput(data, 0.0f, [this](float value) {
              if (cParent.cState == State::RECORDING) {
                cParent.writeEvent(cRuntimeID, value);
              }
              cStateNotifier->stateChanged(this);
            }),
            cRuntimeID(cParent.cDigitalInputs.size() + cParent.cAnalogueInputs.size()),
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
  
  void Replayer::AnalogueInput::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString(metadata.getPropertyData("AnalogueInputName"),  [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, [this](const std::string& value) {return cParent.isInputNameAllowed(*this, value);});
    owner.createPropertyAsset<Float>(metadata.getPropertyData("AnalogueInputValue"), cDefActualInput);
  }
  
  void Replayer::AnalogueInput::setRecordedState(float state) {
    cRuntimeRecordedInput = state;
  }
  
  std::string Replayer::AnalogueInput::getName() const {
    return cDefName;
  }

  float Replayer::AnalogueInput::getValue() const {
    return cParent.cState == State::REPLAYING ? cRuntimeRecordedInput : cDefActualInput->getValue();
  }
  
  bool Replayer::AnalogueInput::renderAssetIcon() const {
    return false;
  }
  
  void Replayer::AnalogueInput::saveAsset(JSONObject object) const {
    // TODO: Implement this.
  }
  
  void Replayer::AnalogueInput::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }
  
  bool Replayer::AnalogueInput::isDefaultConfiguration() const {
    return true;
  }
  
  void Replayer::readEvent() {
    if (cRecording.eof()) {
      // TODO: Action on end of recording???
      return;
    }

    cRecording.read(reinterpret_cast<char*>(&cNextEvent.cID), sizeof(cNextEvent.cID));
    if (cNextEvent.cID < cDigitalInputs.size()) {
      cRecording.read(reinterpret_cast<char*>(&cNextEvent.cState.cDigital), sizeof(cNextEvent.cState.cDigital));
    } else {
      cRecording.read(reinterpret_cast<char*>(&cNextEvent.cState.cAnalogue), sizeof(cNextEvent.cState.cAnalogue));
    }
    cRecording.read(reinterpret_cast<char*>(&cNextEvent.cTime), sizeof(cNextEvent.cTime));
  }
  
  void Replayer::writeEvent(unsigned int id, bool state) {
    cOutput.write(reinterpret_cast<const char*>(&id),           sizeof(id));
    cOutput.write(reinterpret_cast<const char*>(&state),        sizeof(state));
    cOutput.write(reinterpret_cast<const char*>(&cElapsedTime), sizeof(cElapsedTime));
    cOutput.flush();
  }
  
  void Replayer::writeEvent(unsigned int id, float state) {
    cOutput.write(reinterpret_cast<const char*>(&id),           sizeof(id));
    cOutput.write(reinterpret_cast<const char*>(&state),        sizeof(state));
    cOutput.write(reinterpret_cast<const char*>(&cElapsedTime), sizeof(cElapsedTime));
    cOutput.flush();
  }
  
  bool Replayer::isInputNameAllowed(DigitalInput& input, const std::string& name) const {
    for (const std::unique_ptr<DigitalInput>& mInput : cDigitalInputs) {
      if (mInput->getName() == name && mInput.get() != &input) {
        return false;
      }
    }
    for (const std::unique_ptr<AnalogueInput>& mInput : cAnalogueInputs) {
      if (mInput->getName() == name) {
        return false;
      }
    }
    return true;
  }
  
  bool Replayer::isInputNameAllowed(AnalogueInput& input, const std::string& name) const {
    for (const std::unique_ptr<DigitalInput>& mInput : cDigitalInputs) {
      if (mInput->getName() == name) {
        return false;
      }
    }
    for (const std::unique_ptr<AnalogueInput>& mInput : cAnalogueInputs) {
      if (mInput->getName() == name && mInput.get() != &input) {
        return false;
      }
    }
    return true;
  }
  
  const std::string Replayer::JSON_ANALOGUE_INPUTS = "analogueInputs";
  const std::string Replayer::JSON_DIGITAL_INPUTS  = "digitalInputs";
  const std::string Replayer::JSON_NAME            = "name";
  const std::string Replayer::JSON_VALUE           = "value";
}
