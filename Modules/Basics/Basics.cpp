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
#include "Basics.h"

namespace IsoRealms::Basics {
  float Basics::cSoundVolume = 1.0f;
  float Basics::cMusicVolume = 1.0f;

  Basics::Basics(IsoRealms::Project& project, IComponentTypeRegistry& registry):
                    cProject(project),
                    cModule(registry),
                    cActionScript(*this),
                    cSequenceTracks(registry),
                    cComponentTypeAnalogueControl(*this),
                    cComponentTypeBooleanTrigger(*this),
                    cComponentTypeDigitalControl(*this),
                    cComponentTypeFileFont(*this),
                    cComponentTypeFileSound(*this),
                    cComponentTypeFileTexture(*this),
                    cComponentTypeFunction(*this),
                    cComponentTypeInputGroup(*this),
                    cComponentTypeInputSwitch(*this),
                    cComponentTypeInterruptHandler(*this),
                    cComponentTypeProject(*this),
                    cComponentTypeProjectConfigurer(*this),
                    cComponentTypeSequence(*this),
                    cComponentTypeSimpleBoolean(*this),
                    cComponentTypeSimpleColour(*this),
                    cComponentTypeSimpleFloat(*this),
                    cComponentTypeSimpleInteger(*this),
                    cComponentTypeSimpleString(*this),
                    cComponentTypeSimpleVertex(*this),
                    cComponentTypeSprite(*this),
                    cDigitalToAnalogueMapping(registry.getAssetMetadata("DigitalToAnalogue")),
                    cLuaBinding(project.getLuaState(), this) {
    registry.add(&cComponentTypeAnalogueControl,   "AnalogueControl");
    registry.add(&cComponentTypeBooleanTrigger,    "BooleanTrigger");
    registry.add(&cComponentTypeDigitalControl,    "DigitalControl");
    registry.add(&cComponentTypeFileFont,          "Font");
    registry.add(&cComponentTypeFileSound,         "Sound");
    registry.add(&cComponentTypeFileTexture,       "Texture");
    registry.add(&cComponentTypeFunction,          "Function");
    registry.add(&cComponentTypeInputGroup,        "InputGroup");
    registry.add(&cComponentTypeInputSwitch,       "InputSwitch");
    registry.add(&cComponentTypeInterruptHandler,  "InterruptHandler");
    registry.add(&cComponentTypeProject,           "Project");
    registry.add(&cComponentTypeProjectConfigurer, "ProjectConfigurer");
    registry.add(&cComponentTypeSequence,          "Sequence");
    registry.add(&cComponentTypeSimpleBoolean,     "Boolean");
    registry.add(&cComponentTypeSimpleColour,      "Colour");
    registry.add(&cComponentTypeSimpleFloat,       "Float");
    registry.add(&cComponentTypeSimpleInteger,     "Integer");
    registry.add(&cComponentTypeSimpleString,      "String");
    registry.add(&cComponentTypeSimpleVertex,      "Vertex");
    registry.add(&cComponentTypeSprite,            "Sprite");
  }

  void Basics::refreshAssetRegistration(Sequence& sequence) {
    cComponentTypeSequence.refreshAssetRegistration(sequence);
  }

  const Metadata& Basics::getMetadata(const std::string& key) const {
    return cModule.getAssetMetadata(key);
  }

  void Basics::registerAssets(ComponentAssetRegistry& assets) {
    assets.addProvider(&cActionScript, ACTION_SCRIPT, "Run a Script");
    assets.add<IBinding>(&cLuaBinding, "",            "Modules/Basics");
    assets.addProvider(&cDigitalToAnalogueMapping, "DigitalToAnalogue", "Digital to Analogue Mapping");
  }
  
  void Basics::updateInputs(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void Basics::updateRuntime(unsigned int milliseconds) {
    updateRuntime2(cComponentTypeProject,           milliseconds);
    updateRuntime2(cComponentTypeProjectConfigurer, milliseconds);
    updateRuntime2(cComponentTypeSequence,          milliseconds);
  }
  
  void Basics::updateEditing(unsigned int milliseconds) {
    updateEditing2(cComponentTypeSequence, milliseconds);
  }
  
  void Basics::reset() {
    reset2(cComponentTypeDigitalControl);
    reset2(cComponentTypeInputSwitch);
    reset2(cComponentTypeProject);
    reset2(cComponentTypeSequence);
    reset2(cComponentTypeSimpleBoolean);
    reset2(cComponentTypeSimpleColour);
    reset2(cComponentTypeSimpleFloat);
    reset2(cComponentTypeSimpleInteger);
    reset2(cComponentTypeSimpleString);
    reset2(cComponentTypeSimpleVertex);
  }  
  
  IsoRealms::Project& Basics::getProject() const {
    return cProject;
  }

  unsigned int Basics::getAvailableFunctionID() const {
    unsigned int mAvailableID = 0;
    bool mAvailableIDChanged = true;
    while (mAvailableIDChanged) {
      mAvailableIDChanged = false;

      // Check if the function ID is in use by a user function.
      for (Function* mFunction : cComponentTypeFunction) {
        if (mFunction->getID() == mAvailableID) {
          mAvailableID++;
          mAvailableIDChanged = true;
        }
      }
    
      // Check if the function ID is in use by a script action.
      unsigned int mNewAvailableID = cActionScript.getNextAvailableFunctionID(mAvailableID);
      if (mNewAvailableID != mAvailableID) {
        mAvailableID = mNewAvailableID;
        mAvailableIDChanged = true;
      }
    }
    return mAvailableID;
  }

  void Basics::reloadGlobalConfiguration() {
    if (System::fileExists(GLOBAL_CONFIGURATION_FILE, true)) {
      JSONDocument mModuleSettingsDocument(GLOBAL_CONFIGURATION_FILE, true);
      JSONObject mModuleSettingsObject = mModuleSettingsDocument.getObject(JSON_MODULE_SETTINGS);
      setSoundVolume(mModuleSettingsObject.getFloat(JSON_SOUND_VOLUME));
      setMusicVolume(mModuleSettingsObject.getFloat(JSON_MUSIC_VOLUME));
      for (JSONValue mDigitalInputMappingValue : mModuleSettingsObject.getArray(JSON_DIGITAL_INPUT_MAPPINGS)) {
        JSONObject mDigitalInputMappingObject = mDigitalInputMappingValue.getObject();
        std::string mInputID = mDigitalInputMappingObject.getString(JSON_ID);
        DigitalControl* mDigitalInput = cComponentTypeDigitalControl.getComponent(mInputID);
        if (mDigitalInput == nullptr) {
          throw ActionException("ERROR: Basics::reloadGlobalConfiguration: Digital input \"" + mInputID + "\" not found.");
        }
        mDigitalInput->loadCustomMapping(mDigitalInputMappingObject);
      }
      for (JSONValue mAnalogueInputMappingValue : mModuleSettingsObject.getArray(JSON_ANALOGUE_INPUT_MAPPINGS)) {
        JSONObject mAnalogueInputMappingObject = mAnalogueInputMappingValue.getObject();
        std::string mInputID = mAnalogueInputMappingObject.getString(JSON_ID);
        AnalogueControl* mAnalogueInput = cComponentTypeAnalogueControl.getComponent(mInputID);
        if (mAnalogueInput == nullptr) {
          throw ActionException("ERROR: Basics::reloadGlobalConfiguration: Analogue input \"" + mInputID + "\" not found.");
        }
        mAnalogueInput->loadCustomMapping(mAnalogueInputMappingObject);
      }
    }
  }
  
  void Basics::persistGlobalConfiguration() {
    JSONDocument mModuleSettingsDocument;
    JSONObject mModuleSettingsObject =  mModuleSettingsDocument.addObject(JSON_MODULE_SETTINGS);
    mModuleSettingsObject.addFloat(JSON_SOUND_VOLUME, getSoundVolume());
    mModuleSettingsObject.addFloat(JSON_MUSIC_VOLUME, getMusicVolume());
    JSONArray mDigitalInputMappingArray = mModuleSettingsObject.addArray(JSON_DIGITAL_INPUT_MAPPINGS);
    for (DigitalControl* mDigitalInput : cComponentTypeDigitalControl) {
      JSONObject mDigitalInputMappingObject = mDigitalInputMappingArray.addObject();
      mDigitalInputMappingObject.addString(JSON_ID, cComponentTypeDigitalControl.getID(mDigitalInput));
      mDigitalInput->saveCustomMapping(mDigitalInputMappingObject);
    }
    JSONArray mAnalogueInputMappingArray = mModuleSettingsObject.addArray(JSON_ANALOGUE_INPUT_MAPPINGS);
    for (AnalogueControl* mAnalogueInput : cComponentTypeAnalogueControl) {
      JSONObject mAnalogueInputMappingObject = mAnalogueInputMappingArray.addObject();
      mAnalogueInputMappingObject.addString(JSON_ID, cComponentTypeAnalogueControl.getID(mAnalogueInput));
      mAnalogueInput->saveCustomMapping(mAnalogueInputMappingObject);
    }
    mModuleSettingsDocument.save(GLOBAL_CONFIGURATION_FILE);
  }

  float Basics::getSoundVolume() {
    return cSoundVolume;
  }

  void Basics::setSoundVolume(float volume) {
    cSoundVolume = std::clamp(volume, 0.0f, 1.0f);

    // TODO: If multiple projects are loaded, this will only affect the current one.
    for (FileSound* mFileSound : cComponentTypeFileSound) {
      mFileSound->setVolume(cSoundVolume);
    }
  }

  float Basics::getMusicVolume() {
    return cMusicVolume;
  }

  void Basics::setMusicVolume(float volume) {
    cMusicVolume = std::clamp(volume, 0.0f, 1.0f);
  }

  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<Basics>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IComponentTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IComponentTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::Basics::Basics> mModule = std::make_unique<IsoRealms::Basics::Basics>(*project, *registry);
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Basics::cModuleInstantiationMutex);
    return IsoRealms::Basics::ModuleInstances.emplace_back(std::move(mModule)).get();
  }
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::unique_ptr<IsoRealms::Basics::Basics> mModule;
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Basics::cModuleInstantiationMutex);
    mModule = IsoRealms::Utils::removeElementUnique(IsoRealms::Basics::ModuleInstances, module);
  }
}
