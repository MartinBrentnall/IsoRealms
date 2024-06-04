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

namespace IsoRealms::Basics {
  const std::string Basics::ID_RESOURCE_ANALOGUE_INPUT    = "AnalogueInput";
  const std::string Basics::ID_RESOURCE_BOOLEAN_TRIGGER   = "BooleanTrigger";
  const std::string Basics::ID_RESOURCE_COLOUR_CYCLER     = "ColourCycler";
  const std::string Basics::ID_RESOURCE_COLOUR_SWITCH     = "ColourSwitch";
  const std::string Basics::ID_RESOURCE_DIGITAL_INPUT     = "DigitalInput";
  const std::string Basics::ID_RESOURCE_FILE_FONT         = "Font";
  const std::string Basics::ID_RESOURCE_FILE_SOUND        = "Sound";
  const std::string Basics::ID_RESOURCE_FILE_TEXTURE      = "Texture";
  const std::string Basics::ID_RESOURCE_FLOAT_SWITCH      = "FloatSwitch";
  const std::string Basics::ID_RESOURCE_FUNCTION          = "Function";
  const std::string Basics::ID_RESOURCE_INPUT_GROUP       = "InputGroup";
  const std::string Basics::ID_RESOURCE_INPUT_SWITCH      = "InputSwitch";
  const std::string Basics::ID_RESOURCE_INTERRUPT_HANDLER = "InterruptHandler";
  const std::string Basics::ID_RESOURCE_MODEL_CYCLER      = "ModelCycler";
  const std::string Basics::ID_RESOURCE_MODEL_SCREEN      = "ModelScreen";
  const std::string Basics::ID_RESOURCE_PROJECT           = "Project";
  const std::string Basics::ID_RESOURCE_PROJECT_OPTIONS   = "ProjectOptions";
  const std::string Basics::ID_RESOURCE_SEQUENCE          = "Sequence";
  const std::string Basics::ID_RESOURCE_SIMPLE_BOOLEAN    = "Boolean";
  const std::string Basics::ID_RESOURCE_SIMPLE_COLOUR     = "Colour";
  const std::string Basics::ID_RESOURCE_SIMPLE_FLOAT      = "Float";
  const std::string Basics::ID_RESOURCE_SIMPLE_INTEGER    = "Integer";
  const std::string Basics::ID_RESOURCE_SIMPLE_STRING     = "String";
  const std::string Basics::ID_RESOURCE_SIMPLE_VERTEX     = "Vertex";
  const std::string Basics::ID_RESOURCE_SPRITE            = "Sprite";
  const std::string Basics::ID_RESOURCE_STRING_SCREEN     = "StringScreen";
  const std::string Basics::ID_RESOURCE_TIMER             = "Timer";

  const std::string Basics::NAME_RESOURCE_ANALOGUE_INPUT    = "Analogue Inputs";
  const std::string Basics::NAME_RESOURCE_BOOLEAN_TRIGGER   = "Boolean Triggers";
  const std::string Basics::NAME_RESOURCE_COLOUR_CYCLER     = "Simple Cycling Colours";
  const std::string Basics::NAME_RESOURCE_COLOUR_SWITCH     = "Coloured Switches";
  const std::string Basics::NAME_RESOURCE_DIGITAL_INPUT     = "Digital Inputs";
  const std::string Basics::NAME_RESOURCE_FLOAT_SWITCH      = "Float Switches";
  const std::string Basics::NAME_RESOURCE_FILE_FONT         = "Fonts";
  const std::string Basics::NAME_RESOURCE_FILE_SOUND        = "Simple Sounds";
  const std::string Basics::NAME_RESOURCE_FILE_TEXTURE      = "Simple Textures";
  const std::string Basics::NAME_RESOURCE_FUNCTION          = "Functions";
  const std::string Basics::NAME_RESOURCE_INPUT_GROUP       = "Input Groups";
  const std::string Basics::NAME_RESOURCE_INPUT_SWITCH      = "Input Switches";
  const std::string Basics::NAME_RESOURCE_INTERRUPT_HANDLER = "Interrupt Handlers";
  const std::string Basics::NAME_RESOURCE_MODEL_CYCLER      = "Simple Cycleable Models";
  const std::string Basics::NAME_RESOURCE_MODEL_SCREEN      = "Simple Model Screens";
  const std::string Basics::NAME_RESOURCE_PROJECT           = "Projects";
  const std::string Basics::NAME_RESOURCE_PROJECT_OPTIONS   = "Project Options";
  const std::string Basics::NAME_RESOURCE_SEQUENCE          = "Sequences";
  const std::string Basics::NAME_RESOURCE_SIMPLE_BOOLEAN    = "Simple Booleans";
  const std::string Basics::NAME_RESOURCE_SIMPLE_COLOUR     = "Simple Colours";
  const std::string Basics::NAME_RESOURCE_SIMPLE_FLOAT      = "Simple Floats";
  const std::string Basics::NAME_RESOURCE_SIMPLE_INTEGER    = "Simple Integers";
  const std::string Basics::NAME_RESOURCE_SIMPLE_STRING     = "Simple Strings";
  const std::string Basics::NAME_RESOURCE_SIMPLE_VERTEX     = "Simple Vertices";
  const std::string Basics::NAME_RESOURCE_SPRITE            = "Simple Sprite Models";
  const std::string Basics::NAME_RESOURCE_STRING_SCREEN     = "Simple String Screens";
  const std::string Basics::NAME_RESOURCE_TIMER             = "Timers";

  const std::string Basics::ACTION_TYPE_SCRIPT                = "Script";
  const std::string Basics::SEQUENCE_TRACK_TYPE_ACTION        = "Action";
  const std::string Basics::SEQUENCE_TRACK_TYPE_COLOUR        = "Colour";

  const std::string Basics::JSON_ANALOGUE_INPUT_MAPPINGS = "analogueInputMappings";
  const std::string Basics::JSON_DIGITAL_INPUT_MAPPINGS  = "digitalInputMappings";
  const std::string Basics::JSON_ID                      = "id";
  const std::string Basics::JSON_MODULE_SETTINGS         = "moduleSettings";
  const std::string Basics::JSON_MUSIC_VOLUME            = "musicVolume";
  const std::string Basics::JSON_SOUND_VOLUME            = "soundVolume";

  const std::string Basics::GLOBAL_CONFIGURATION_FILE         = "Module_IsoRealms.json";

  float Basics::cSoundVolume = 1.0f;
  float Basics::cMusicVolume = 1.0f;

  Basics::Basics(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals):
                    cResourceTypeAnalogueInput(this),
                    cResourceTypeBooleanTrigger(this),
                    cResourceTypeColourCycler(this),
                    cResourceTypeColourSwitch(this),
                    cResourceTypeDigitalInput(this),
                    cResourceTypeFileFont(this),
                    cResourceTypeFileSound(this),
                    cResourceTypeFileTexture(this),
                    cResourceTypeFloatSwitch(this),
                    cResourceTypeFunction(this),
                    cResourceTypeInputGroup(this),
                    cResourceTypeInputSwitch(this),
                    cResourceTypeInterruptHandler(this),
                    cResourceTypeModelCycler(this),
                    cResourceTypeModelScreen(this),
                    cResourceTypeProject(this),
                    cResourceTypeProjectOptions(this),
                    cResourceTypeSequence(this),
                    cResourceTypeSimpleBoolean(this),
                    cResourceTypeSimpleColour(this),
                    cResourceTypeSimpleFloat(this),
                    cResourceTypeSimpleInteger(this),
                    cResourceTypeSimpleString(this),
                    cResourceTypeSimpleVertex(this),
                    cResourceTypeSprite(this),
                    cResourceTypeStringScreen(this),
                    cResourceTypeTimer(this),
                    cActionTypeScript(),
                    cLuaBinding(project, this) {
    registry->add(&cResourceTypeAnalogueInput,    ID_RESOURCE_ANALOGUE_INPUT,    NAME_RESOURCE_ANALOGUE_INPUT,    IsoRealmsConstants::RESOURCE_CATEGORY_LOGIC_AND_INPUT);
    registry->add(&cResourceTypeBooleanTrigger,   ID_RESOURCE_BOOLEAN_TRIGGER,   NAME_RESOURCE_BOOLEAN_TRIGGER,   IsoRealmsConstants::RESOURCE_CATEGORY_LOGIC_AND_INPUT);
    registry->add(&cResourceTypeColourCycler,     ID_RESOURCE_COLOUR_CYCLER,     NAME_RESOURCE_COLOUR_CYCLER,     IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_GRAPHICS);
    registry->add(&cResourceTypeColourSwitch,     ID_RESOURCE_COLOUR_SWITCH,     NAME_RESOURCE_COLOUR_SWITCH,     IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeDigitalInput,     ID_RESOURCE_DIGITAL_INPUT,     NAME_RESOURCE_DIGITAL_INPUT,     IsoRealmsConstants::RESOURCE_CATEGORY_LOGIC_AND_INPUT);
    registry->add(&cResourceTypeFileFont,         ID_RESOURCE_FILE_FONT,         NAME_RESOURCE_FILE_FONT,         IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeFileSound,        ID_RESOURCE_FILE_SOUND,        NAME_RESOURCE_FILE_SOUND,        IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeFileTexture,      ID_RESOURCE_FILE_TEXTURE,      NAME_RESOURCE_FILE_TEXTURE,      IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_GRAPHICS);
    registry->add(&cResourceTypeFloatSwitch,      ID_RESOURCE_FLOAT_SWITCH,      NAME_RESOURCE_FLOAT_SWITCH,      IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeFunction,         ID_RESOURCE_FUNCTION,          NAME_RESOURCE_FUNCTION,          IsoRealmsConstants::RESOURCE_CATEGORY_LOGIC_AND_INPUT);
    registry->add(&cResourceTypeInputGroup,       ID_RESOURCE_INPUT_GROUP,       NAME_RESOURCE_INPUT_GROUP,       IsoRealmsConstants::RESOURCE_CATEGORY_LOGIC_AND_INPUT);
    registry->add(&cResourceTypeInputSwitch,      ID_RESOURCE_INPUT_SWITCH,      NAME_RESOURCE_INPUT_SWITCH,      IsoRealmsConstants::RESOURCE_CATEGORY_LOGIC_AND_INPUT);
    registry->add(&cResourceTypeInterruptHandler, ID_RESOURCE_INTERRUPT_HANDLER, NAME_RESOURCE_INTERRUPT_HANDLER, IsoRealmsConstants::RESOURCE_CATEGORY_LOGIC_AND_INPUT);
    registry->add(&cResourceTypeModelCycler,      ID_RESOURCE_MODEL_CYCLER,      NAME_RESOURCE_MODEL_CYCLER,      IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_GRAPHICS);
    registry->add(&cResourceTypeModelScreen,      ID_RESOURCE_MODEL_SCREEN,      NAME_RESOURCE_MODEL_SCREEN,      IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeProject,          ID_RESOURCE_PROJECT,           NAME_RESOURCE_PROJECT,           IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
    registry->add(&cResourceTypeProjectOptions,   ID_RESOURCE_PROJECT_OPTIONS,   NAME_RESOURCE_PROJECT_OPTIONS,   IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
    registry->add(&cResourceTypeSequence,         ID_RESOURCE_SEQUENCE,          NAME_RESOURCE_SEQUENCE,          IsoRealmsConstants::RESOURCE_CATEGORY_LOGIC_AND_INPUT);
    registry->add(&cResourceTypeSimpleBoolean,    ID_RESOURCE_SIMPLE_BOOLEAN,    NAME_RESOURCE_SIMPLE_BOOLEAN,    IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleColour,     ID_RESOURCE_SIMPLE_COLOUR,     NAME_RESOURCE_SIMPLE_COLOUR,     IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_GRAPHICS);
    registry->add(&cResourceTypeSimpleFloat,      ID_RESOURCE_SIMPLE_FLOAT,      NAME_RESOURCE_SIMPLE_FLOAT,      IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleInteger,    ID_RESOURCE_SIMPLE_INTEGER,    NAME_RESOURCE_SIMPLE_INTEGER,    IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleString,     ID_RESOURCE_SIMPLE_STRING,     NAME_RESOURCE_SIMPLE_STRING,     IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSimpleVertex,     ID_RESOURCE_SIMPLE_VERTEX,     NAME_RESOURCE_SIMPLE_VERTEX,     IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
    registry->add(&cResourceTypeSprite,           ID_RESOURCE_SPRITE,            NAME_RESOURCE_SPRITE,            IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_GRAPHICS);
    registry->add(&cResourceTypeStringScreen,     ID_RESOURCE_STRING_SCREEN,     NAME_RESOURCE_STRING_SCREEN,     IsoRealmsConstants::RESOURCE_CATEGORY_PRESENTATION);
    registry->add(&cResourceTypeTimer,            ID_RESOURCE_TIMER,             NAME_RESOURCE_TIMER,             IsoRealmsConstants::RESOURCE_CATEGORY_DYNAMIC_VARIABLES);
  }

  void Basics::load(IProject* project, JSONObject object) {
    // Nothing to do.
  }

  void Basics::save(JSONObject object, IAssetIdentifier* identifier) {
    // Nothing to do.
  }

  void Basics::registerAssets(IAssetRegistry* assets) {
    assets->add(&cActionTypeScript, ACTION_TYPE_SCRIPT, "");
    assets->add(&cLuaBinding,       "",                 "Basics");
  }
  
  void Basics::unregisterAssets(IAssetRemover* remover, IAssets* releaser) {
    cActionTypeScript.unregisterAssets(remover, releaser);
    remover->remove(&cLuaBinding);
  }

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
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry, IsoRealms::IAssetLiterals* literals) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry, IsoRealms::IAssetLiterals * literals) {
#endif
  std::unique_ptr<IsoRealms::Basics::Basics> mModule = std::make_unique<IsoRealms::Basics::Basics>(project, registry, literals);
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
