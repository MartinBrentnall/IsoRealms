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
#include "Basics.h"

#include "IsoRealms/Project.h"

namespace IsoRealms::Basics {
  const std::string Basics::ID_RESOURCE_ANALOGUE_INPUT     = "AnalogueInput";
  const std::string Basics::ID_RESOURCE_BOOLEAN_TRIGGER    = "BooleanTrigger";
  const std::string Basics::ID_RESOURCE_DIGITAL_INPUT      = "DigitalInput";
  const std::string Basics::ID_RESOURCE_FILE_FONT          = "Font";
  const std::string Basics::ID_RESOURCE_FILE_SOUND         = "Sound";
  const std::string Basics::ID_RESOURCE_FILE_TEXTURE       = "Texture";
  const std::string Basics::ID_RESOURCE_FUNCTION           = "Function";
  const std::string Basics::ID_RESOURCE_INPUT_GROUP        = "InputGroup";
  const std::string Basics::ID_RESOURCE_INPUT_SWITCH       = "InputSwitch";
  const std::string Basics::ID_RESOURCE_INTERRUPT_HANDLER  = "InterruptHandler";
  const std::string Basics::ID_RESOURCE_PROJECT            = "Project";
  const std::string Basics::ID_RESOURCE_PROJECT_CONFIGURER = "ProjectConfigurer";
  const std::string Basics::ID_RESOURCE_PROJECT_OPTIONS    = "ProjectOptions";
  const std::string Basics::ID_RESOURCE_SEQUENCE           = "Sequence";
  const std::string Basics::ID_RESOURCE_SIMPLE_BOOLEAN     = "Boolean";
  const std::string Basics::ID_RESOURCE_SIMPLE_COLOUR      = "Colour";
  const std::string Basics::ID_RESOURCE_SIMPLE_FLOAT       = "Float";
  const std::string Basics::ID_RESOURCE_SIMPLE_INTEGER     = "Integer";
  const std::string Basics::ID_RESOURCE_SIMPLE_STRING      = "String";
  const std::string Basics::ID_RESOURCE_SIMPLE_VERTEX      = "Vertex";
  const std::string Basics::ID_RESOURCE_SPRITE             = "Sprite";

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

  const std::string Basics::GLOBAL_CONFIGURATION_FILE         = "Module_IsoRealms.json";

  float Basics::cSoundVolume = 1.0f;
  float Basics::cMusicVolume = 1.0f;

  Basics::Basics(IProject& project, IResourceTypeRegistry* registry):
                    cProject(project),
                    cProviderSequenceTrackAction(project),
                    cProviderSequenceTrackAudio(project),
                    cProviderSequenceTrackColour(project),
                    cProviderSequenceTrackFloat(project),
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
                    cActionScript(),
                    cLuaBinding(project, this) {
    registry->add(&cResourceTypeAnalogueInput,     ID_RESOURCE_ANALOGUE_INPUT,     "Input (Analogue)",   "Inputs (Analogue)",    "Input");
    registry->add(&cResourceTypeBooleanTrigger,    ID_RESOURCE_BOOLEAN_TRIGGER,    "Boolean Trigger",    "Boolean Triggers",     "Logic");
    registry->add(&cResourceTypeDigitalInput,      ID_RESOURCE_DIGITAL_INPUT,      "Input (Digital)",    "Inputs (Digital)",     "Input");
    registry->add(&cResourceTypeFileFont,          ID_RESOURCE_FILE_FONT,          "Font",               "Fonts",                IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeFileSound,         ID_RESOURCE_FILE_SOUND,         "Sound",              "Sounds",               IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeFileTexture,       ID_RESOURCE_FILE_TEXTURE,       "Image",              "Images",               IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeFunction,          ID_RESOURCE_FUNCTION,           "Function",           "Functions",            "Logic");
    registry->add(&cResourceTypeInputGroup,        ID_RESOURCE_INPUT_GROUP,        "Input Group",        "Input Groups",         "Input");
    registry->add(&cResourceTypeInputSwitch,       ID_RESOURCE_INPUT_SWITCH,       "Input Switch",       "Input Switches",       "Input");
    registry->add(&cResourceTypeInterruptHandler,  ID_RESOURCE_INTERRUPT_HANDLER,  "Interrupt Handler",  "Interrupt Handlers",   "Input");
    registry->add(&cResourceTypeProject,           ID_RESOURCE_PROJECT,            "Project",            "Projects",             IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
    registry->add(&cResourceTypeProjectConfigurer, ID_RESOURCE_PROJECT_CONFIGURER, "Project Configurer", "Project Configurers",  IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
    registry->add(&cResourceTypeProjectOptions,    ID_RESOURCE_PROJECT_OPTIONS,    "Project Option Set", "Project Options Sets", IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
    registry->add(&cResourceTypeSequence,          ID_RESOURCE_SEQUENCE,           "Sequence",           "Sequences",            "Logic");
    registry->add(&cResourceTypeSimpleBoolean,     ID_RESOURCE_SIMPLE_BOOLEAN,     "Boolean",            "Booleans",             IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleColour,      ID_RESOURCE_SIMPLE_COLOUR,      "Colour",             "Colours",              IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleFloat,       ID_RESOURCE_SIMPLE_FLOAT,       "Float",              "Floats",               IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleInteger,     ID_RESOURCE_SIMPLE_INTEGER,     "Integer",            "Integers",             IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleString,      ID_RESOURCE_SIMPLE_STRING,      "String",             "Strings",              IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleVertex,      ID_RESOURCE_SIMPLE_VERTEX,      "Vertex",             "Vertices",             IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSprite,            ID_RESOURCE_SPRITE,             "Sprite",             "Sprites",              IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);

    // Register Basics built-in asset providers.
    cSequenceTracks.add(&cProviderSequenceTrackAction, SEQUENCE_TRACK_ACTION, "Basics");
    cSequenceTracks.add(&cProviderSequenceTrackAudio,  SEQUENCE_TRACK_AUDIO,  "Basics");
    cSequenceTracks.add(&cProviderSequenceTrackColour, SEQUENCE_TRACK_COLOUR, "Basics");
    cSequenceTracks.add(&cProviderSequenceTrackFloat,  SEQUENCE_TRACK_FLOAT,  "Basics");
  }

  void Basics::refreshAssetRegistration(Sequence& sequence) {
    cResourceTypeSequence.refreshAssetRegistration(sequence);
  }

  void Basics::load(IProject& project, JSONObject object) {
    // Nothing to do.
  }

  void Basics::save(JSONObject object) {
    // Nothing to do.
  }

  void Basics::registerAssets(IAssetRegistry& assets) {
    assets.add(&cActionScript, ACTION_SCRIPT, "");
    assets.add(&cLuaBinding,   "",            "Basics");
  }
  
  std::vector<std::unique_ptr<IProperty>> Basics::getProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
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

  IProject& Basics::getProject() const {
    return cProject;
  }

  bool Basics::isReadOnly() const {
    return false;
  } // TODO: Probably shouldn't be here.

  void Basics::setOwner(File* owner) {
  }; // TODO: Probably shouldn't be here.

  ISequenceTrack* Basics::createLiteralSequenceTrack(IAssetUser<ISequenceTrack>* user, Sequence& owner) {return cSequenceTracks.literal(user, owner, "");}

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
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::Basics::Basics> mModule = std::make_unique<IsoRealms::Basics::Basics>(*project, registry);
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
