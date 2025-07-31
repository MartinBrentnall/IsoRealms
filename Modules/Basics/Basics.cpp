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
  const std::string Basics::SEQUENCE_TRACK_ACTION = "Action";
  const std::string Basics::SEQUENCE_TRACK_AUDIO  = "Audio";
  const std::string Basics::SEQUENCE_TRACK_COLOUR = "Colour";
  const std::string Basics::SEQUENCE_TRACK_FLOAT  = "Float";

  const std::string Basics::ACTION_SCRIPT = "Script";
  
  const std::string Basics::JSON_ANALOGUE_INPUT_MAPPINGS = "analogueInputMappings";
  const std::string Basics::JSON_DIGITAL_INPUT_MAPPINGS  = "digitalInputMappings";
  const std::string Basics::JSON_ID                      = "id";
  const std::string Basics::JSON_MODULE_SETTINGS         = "moduleSettings";
  const std::string Basics::JSON_MUSIC_VOLUME            = "musicVolume";
  const std::string Basics::JSON_SOUND_VOLUME            = "soundVolume";

  const std::string Basics::GLOBAL_CONFIGURATION_FILE = "Module_IsoRealms.json";

  float Basics::cSoundVolume = 1.0f;
  float Basics::cMusicVolume = 1.0f;

  Basics::Basics(IsoRealms::Project& project, IResourceTypeRegistry& registry):
                    cProject(project),
                    cModule(registry),
                    cProviderSequenceTrackAction(registry.getAssetMetadata("SequenceTrackAction")),
                    cProviderSequenceTrackAudio(registry.getAssetMetadata("SequenceTrackAudio")),
                    cProviderSequenceTrackColour(registry.getAssetMetadata("SequenceTrackColour")),
                    cProviderSequenceTrackFloat(registry.getAssetMetadata("SequenceTrackFloat")),
                    cResourceTypeAnalogueInput(*this),
                    cResourceTypeBooleanTrigger(*this),
                    cResourceTypeDigitalInput(*this),
                    cResourceTypeFileFont(*this),
                    cResourceTypeFileSound(*this),
                    cResourceTypeFileTexture(*this),
                    cResourceTypeFunction(*this),
                    cResourceTypeInputGroup(*this),
                    cResourceTypeInputSwitch(*this),
                    cResourceTypeInterruptHandler(*this),
                    cResourceTypeProject(*this),
                    cResourceTypeProjectConfigurer(*this),
                    cResourceTypeProjectOptions(*this),
                    cResourceTypeSequence(*this),
                    cResourceTypeSimpleBoolean(*this),
                    cResourceTypeSimpleColour(*this),
                    cResourceTypeSimpleFloat(*this),
                    cResourceTypeSimpleInteger(*this),
                    cResourceTypeSimpleString(*this),
                    cResourceTypeSimpleVertex(*this),
                    cResourceTypeSprite(*this),
                    cActionScript(*this),
                    cLuaBinding(project, this) {
    registry.add(&cResourceTypeAnalogueInput,     "AnalogueInput");
    registry.add(&cResourceTypeBooleanTrigger,    "BooleanTrigger");
    registry.add(&cResourceTypeDigitalInput,      "DigitalInput");
    registry.add(&cResourceTypeFileFont,          "Font");
    registry.add(&cResourceTypeFileSound,         "Sound");
    registry.add(&cResourceTypeFileTexture,       "Texture");
    registry.add(&cResourceTypeFunction,          "Function");
    registry.add(&cResourceTypeInputGroup,        "InputGroup");
    registry.add(&cResourceTypeInputSwitch,       "InputSwitch");
    registry.add(&cResourceTypeInterruptHandler,  "InterruptHandler");
    registry.add(&cResourceTypeProject,           "Project");
    registry.add(&cResourceTypeProjectConfigurer, "ProjectConfigurer");
    registry.add(&cResourceTypeProjectOptions,    "ProjectOptions");
    registry.add(&cResourceTypeSequence,          "Sequence");
    registry.add(&cResourceTypeSimpleBoolean,     "Boolean");
    registry.add(&cResourceTypeSimpleColour,      "Colour");
    registry.add(&cResourceTypeSimpleFloat,       "Float");
    registry.add(&cResourceTypeSimpleInteger,     "Integer");
    registry.add(&cResourceTypeSimpleString,      "String");
    registry.add(&cResourceTypeSimpleVertex,      "Vertex");
    registry.add(&cResourceTypeSprite,            "Sprite");

    // Register Basics built-in asset providers.
    cSequenceTracks.add(&cProviderSequenceTrackAction, SEQUENCE_TRACK_ACTION, "Basics");
    cSequenceTracks.add(&cProviderSequenceTrackAudio,  SEQUENCE_TRACK_AUDIO,  "Basics");
    cSequenceTracks.add(&cProviderSequenceTrackColour, SEQUENCE_TRACK_COLOUR, "Basics");
    cSequenceTracks.add(&cProviderSequenceTrackFloat,  SEQUENCE_TRACK_FLOAT,  "Basics");
  }

  void Basics::refreshAssetRegistration(Sequence& sequence) {
    cResourceTypeSequence.refreshAssetRegistration(sequence);
  }

  const Metadata& Basics::getMetadata(const std::string& key) const {
    return cModule.getAssetMetadata(key);
  }

  void Basics::load(IProject& project, JSONObject object) {
    // Nothing to do.
  }

  void Basics::save(JSONObject object) {
    // Nothing to do.
  }

  void Basics::registerAssets(ResourceAssetRegistry& assets) {
    assets.addProvider(&cActionScript, ACTION_SCRIPT, "");
    assets.add<IBinding>(&cLuaBinding,   "",            "Basics");
  }
  
  void Basics::getProperties() {
    // Nothing to do.
  }
  
  void Basics::updateRuntime(unsigned int milliseconds) {
    updateRuntime2(cResourceTypeProject,           milliseconds);
    updateRuntime2(cResourceTypeProjectConfigurer, milliseconds);
    updateRuntime2(cResourceTypeSequence,          milliseconds);
  }
  
  void Basics::updateEditing(unsigned int milliseconds) {
    updateEditing2(cResourceTypeSequence, milliseconds);
  }
  
  void Basics::reset() {
    reset2(cResourceTypeDigitalInput);
    reset2(cResourceTypeInputSwitch);
    reset2(cResourceTypeProject);
    reset2(cResourceTypeSequence);
    reset2(cResourceTypeSimpleBoolean);
    reset2(cResourceTypeSimpleColour);
    reset2(cResourceTypeSimpleFloat);
    reset2(cResourceTypeSimpleInteger);
    reset2(cResourceTypeSimpleString);
    reset2(cResourceTypeSimpleVertex);
  }  
  
  Basics& Basics::getAssetManager() {
    return *this;
  }

  IsoRealms::Project& Basics::getProject() const {
    return cProject;
  }

  bool Basics::isReadOnly() const {
    return false;
  } // TODO: Probably shouldn't be here.

  void Basics::setOwner(ProjectFile* owner) {
  }; // TODO: Probably shouldn't be here.

  ISequenceTrack* Basics::getSequenceTrack(IAssetUser<ISequenceTrack>* user, JSONObject object, Sequence& owner) {return cSequenceTracks.get(user, owner, object, nullptr, true, [this](JSONObject object, IStateListener<ISequenceTrack*>* listener) -> ISequenceTrack* {return nullptr;});}

  ISequenceTrack* Basics::getSequenceTrack(IAssetUser<ISequenceTrack>* user, const std::string& id, Sequence& owner) {return cSequenceTracks.get(user, owner, id, nullptr);}

  void Basics::reloadGlobalConfiguration() {
    if (System::fileExists(GLOBAL_CONFIGURATION_FILE, true)) {
      JSONDocument mModuleSettingsDocument(GLOBAL_CONFIGURATION_FILE, true);
      JSONObject mModuleSettingsObject = mModuleSettingsDocument.getObject(JSON_MODULE_SETTINGS);
      setSoundVolume(mModuleSettingsObject.getFloat(JSON_SOUND_VOLUME));
      setMusicVolume(mModuleSettingsObject.getFloat(JSON_MUSIC_VOLUME));
      for (JSONObject mDigitalInputMappingObject : mModuleSettingsObject.getArray(JSON_DIGITAL_INPUT_MAPPINGS)) {
        std::string mInputID = mDigitalInputMappingObject.getString(JSON_ID);
        DigitalInput* mDigitalInput = cResourceTypeDigitalInput.getResource(mInputID);
        if (mDigitalInput == nullptr) {
          throw ActionException("ERROR: Basics::reloadGlobalConfiguration: Digital input \"" + mInputID + "\" not found.");
        }
        mDigitalInput->loadCustomMapping(mDigitalInputMappingObject);
      }
      for (JSONObject mAnalogueInputMappingObject : mModuleSettingsObject.getArray(JSON_ANALOGUE_INPUT_MAPPINGS)) {
        std::string mInputID = mAnalogueInputMappingObject.getString(JSON_ID);
        AnalogueInput* mAnalogueInput = cResourceTypeAnalogueInput.getResource(mInputID);
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
    for (DigitalInput* mDigitalInput : cResourceTypeDigitalInput) {
      JSONObject mDigitalInputMappingObject = mDigitalInputMappingArray.addObject();
      mDigitalInputMappingObject.addString(JSON_ID, cResourceTypeDigitalInput.getID(mDigitalInput));
      mDigitalInput->saveCustomMapping(mDigitalInputMappingObject);
    }
    JSONArray mAnalogueInputMappingArray = mModuleSettingsObject.addArray(JSON_ANALOGUE_INPUT_MAPPINGS);
    for (AnalogueInput* mAnalogueInput : cResourceTypeAnalogueInput) {
      JSONObject mAnalogueInputMappingObject = mAnalogueInputMappingArray.addObject();
      mAnalogueInputMappingObject.addString(JSON_ID, cResourceTypeAnalogueInput.getID(mAnalogueInput));
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
    for (FileSound* mFileSound : cResourceTypeFileSound) {
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
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IResourceTypeRegistry * registry) {
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
