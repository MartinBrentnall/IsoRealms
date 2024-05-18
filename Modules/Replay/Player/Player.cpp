/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Player.h"

#include "Modules/Replay/Recorder/Recorder.h"

namespace IsoRealms::Replay {
  const std::string Player::JSON_INPUT               = "input";
  const std::string Player::JSON_INPUT_CONFIGURATION = "inputConfiguration";
  const std::string Player::JSON_INPUTS              = "inputs";
  const std::string Player::JSON_KEY                 = "key";
  const std::string Player::JSON_ON_FINISH           = "onFinish";
  const std::string Player::JSON_TYPE                = "type";

  const std::string Player::TYPE_ANALOGUE = "Analogue";
  const std::string Player::TYPE_DIGITAL  = "Digital";

  Player::Player(IProject* project, Replay* replay) :
            cParentProject(project),
            cFilename(""),
            cQuitAction(project) {
    cElapsedTime = 0;
    cFinished    = false;
  }
  
  Player::Player(IProject* project, Replay* replay, JSONObject object, IOptions* options, IResourceData* data) :
            Player(project, replay) {

    // Read the recording file
    bool mUserRecording = options->getOption("User") == "true";
    cFilename  = options->getOption("Recording");
    cFilename  = System::getPath(cFilename, mUserRecording);
    cRecording = std::ifstream(cFilename, std::ios::binary);

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

    // Read the inputs to listen to and add digital listeners to those inputs.
    JSONDocument mInputConfigurationDocument(mConfigurationFile, false);
    JSONObject mInputConfigurationObject = mInputConfigurationDocument.getObject(JSON_INPUT_CONFIGURATION);
    for (JSONObject mInputObject : mInputConfigurationObject.getArray(JSON_INPUTS)) {
      std::string mInputType = mInputObject.getString(JSON_TYPE);
      std::string mInputKey = mInputObject.getObject(JSON_INPUT).getString(JSON_KEY);
      if (mInputType == TYPE_DIGITAL) {
        cOverriddenDigitalInputs[mInputKey] = cDigitalInputs.emplace_back(std::make_unique<DigitalInput>()).get();
      } else if (mInputType == TYPE_ANALOGUE) {
        cOverriddenAnalogueInputs[mInputKey] = cAnalogueInputs.emplace_back(std::make_unique<AnalogueInput>()).get();
      } else {
        // TODO: Throw.
      }
    }

    // Open project.
    LocalOptions mLocalOptions("Project", options);
    WrappedOptions mWrappedOptions(&mLocalOptions);
    mWrappedOptions.addOption("file", mProjectFile);
    mWrappedOptions.addOption("type", "program");
    cProject = std::make_unique<Project>(cParentProject->getApplication(), &mWrappedOptions, [this](bool quitRequestGranted) {
      cRecording.close();
      cQuitAction.execute();
    }, this); // TODO: 'user' flag shouldn't always be false

    project->updateRuntime([this](unsigned int milliseconds) {
      while (!cFinished && cNextEvent.cTime == cElapsedTime) {
        if (cNextEvent.cID < cDigitalInputs.size()) {
          cDigitalInputs[cNextEvent.cID]->setValue(cNextEvent.cState.cDigital);
        } else {
          cAnalogueInputs[cNextEvent.cID - cDigitalInputs.size()]->setValue(cNextEvent.cState.cAnalogue);
        }
        readNextEvent();
      }
      cElapsedTime += milliseconds;
      cProject->updateRuntime(milliseconds);
      cProject->updateRuntimeComplete();
    });

    project->reset([this]() {
      cProject->reset();
      cElapsedTime = 0;
      cRecording   = std::ifstream(cFilename, std::ios::binary);
      cFinished    = false;

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
        mInput->setValue(false);
      }
      for (std::unique_ptr<AnalogueInput>& mInput : cAnalogueInputs) {
        mInput->setValue(0.0f);
      }

      // Read first event.
      readNextEvent();
    });

    project->mainThreadInit([this]() {
      cProject->initMainThread();
    });

    cQuitAction.init(object, JSON_ON_FINISH);
  }

  bool Player::renderAssetIcon() const {
    return false;
  }

  void Player::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  bool Player::renderIcon() const {
    return false;
  }

  void Player::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void Player::registerAssets(IAssetRegistry* assets) {
    assets->add(static_cast<IAssets*>(cProject.get()), "Project", "System");
    assets->add(static_cast<IScreen*>(this), "", "System");
    assets->add(static_cast<IInputHandler*>(this), "", "System");
  }
  
  void Player::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IAssets*>(cProject.get()));
    assets->remove(static_cast<IScreen*>(this));
    assets->remove(static_cast<IInputHandler*>(this));
  }
  
  std::vector<IProperty*> Player::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void Player::finish() {
    if (!cFinished) {
      cFinished = true;
      cProject->finish(false);
    }
  }

  void Player::readNextEvent() {
    if (cRecording.eof()) {
      finish();
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

  
  void Player::renderScreen(float scale, float aspectRatio) const {
    cProject->render(aspectRatio);
  }

  bool Player::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Escape: cRecording.close(); return true;
          default:                                       break;
        }
      }
      default: break;
    }
    return false;
  }

  I3DModelType* Player::getModelType(JSONObject object, IStateListener<I3DModelType*>* listener) const {
    return nullptr;
  }

  IActionType* Player::getActionType(JSONObject object, IStateListener<IActionType*>* listener) const {
    return nullptr;
  }

  IAssets* Player::getAssets(JSONObject object, IStateListener<IAssets*>* listener) const {
    return nullptr;
  }

  IBinding* Player::getBinding(JSONObject object, IStateListener<IBinding*>* listener) const {
    return nullptr;
  }

  IBoolean* Player::getBoolean(JSONObject object, IStateListener<IBoolean*>* listener) const {
    std::string mValue = object.getString(JSON_KEY);
    std::map<std::string, DigitalInput*>::const_iterator mInput = cOverriddenDigitalInputs.find(mValue);
    if (mInput != cOverriddenDigitalInputs.end()) {
      if (listener != nullptr) {
        mInput->second->addListener(listener);
      }
      return mInput->second;
    }
    return nullptr;
  }

  IColour* Player::getColour(JSONObject object, IStateListener<IColour*>* listener) const {
    return nullptr;
  }

  IEditable* Player::getEditable(JSONObject object, IStateListener<IEditable*>* listener) const {
    return nullptr;
  }

  IFloat* Player::getFloat(JSONObject object, IStateListener<IFloat*>* listener) const {
    std::string mValue = object.getString(JSON_KEY);
    std::map<std::string, IFloat*>::const_iterator mInput = cOverriddenAnalogueInputs.find(mValue);
    return mInput != cOverriddenAnalogueInputs.end() ? mInput->second : nullptr;
  }

  IFont* Player::getFont(JSONObject object, IStateListener<IFont*>* listener) const {
    return nullptr;
  }

  IInputHandler* Player::getInputHandler(JSONObject object, IStateListener<IInputHandler*>* listener) const {
    return nullptr;
  }

  IInteger* Player::getInteger(JSONObject object, IStateListener<IInteger*>* listener) const {
    return nullptr;
  }

  IProjectOptions* Player::getProjectOptions(JSONObject object, IStateListener<IProjectOptions*>* listener) const {
    return nullptr;
  }

  IScreen* Player::getScreen(JSONObject object, IStateListener<IScreen*>* listener) const {
    return nullptr;
  }

  IString* Player::getString(JSONObject object, IStateListener<IString*>* listener) const {
    return nullptr;
  }

  ITexture* Player::getTexture(JSONObject object, IStateListener<ITexture*>* listener) const {
    return nullptr;
  }

  IVertex* Player::getVertex(JSONObject object, IStateListener<IVertex*>* listener) const {
    return nullptr;
  }

  Player::DigitalInput::DigitalInput() {
    cState = false;
  }
  
  void Player::DigitalInput::setValue(bool state) {
    if (cState != state) {
      cState = state;
      for (IStateListener<IBoolean*>* mListener : cListeners) {
        mListener->stateChanged(this);
      }
    }
  }
  
  void Player::DigitalInput::addListener(IStateListener<IBoolean*>* listener) {
    cListeners.emplace_back(listener);
  }

  bool Player::DigitalInput::getValue() const {
    return cState;
  }

  bool Player::DigitalInput::renderAssetIcon() const {
    return false;
  }

  void Player::DigitalInput::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  Player::AnalogueInput::AnalogueInput() {
    cState = false;
  }

  void Player::AnalogueInput::setValue(float state) {
    cState = state;
  }

  float Player::AnalogueInput::getValue() const {
    return cState;
  }

  bool Player::AnalogueInput::renderAssetIcon() const {
    return false;
  }

  void Player::AnalogueInput::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Player::save(JSONObject object, IAssetIdentifier* identifier) const {
    cQuitAction.save(object, JSON_ON_FINISH);
  }
}
