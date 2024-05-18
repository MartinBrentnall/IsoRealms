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
#pragma once

#include "IsoRealms/IModuleHandle.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceTypeRegistry.h"
#include "IsoRealms/IsoRealmsConstants.h"
#include "IsoRealms/ResourceTypeDefinition.h"

#include "AnalogueInput/AnalogueInput.h"
#include "BooleanTrigger/BooleanTrigger.h"
#include "ColourCycler/ColourCycler.h"
#include "ColourSwitch/ColourSwitch.h"
#include "DigitalInput/DigitalInput.h"
#include "FileFont/FileFont.h"
#include "FileSound/FileSound.h"
#include "FileTexture/FileTexture.h"
#include "FloatSwitch/FloatSwitch.h"
#include "Function/Function.h"
#include "InputGroup/InputGroup.h"
#include "InputSwitch/InputSwitch.h"
#include "InterruptHandler/InterruptHandler.h"
#include "ModelCycler/ModelCycler.h"
#include "ModelScreen/ModelScreen.h"
#include "Project/Project.h"
#include "ProjectOptions/ProjectOptions.h"
#include "Sequence/Sequence.h"
#include "SimpleBoolean/SimpleBoolean.h"
#include "SimpleColour/SimpleColour.h"
#include "SimpleFloat/SimpleFloat.h"
#include "SimpleInteger/SimpleInteger.h"
#include "SimpleString/SimpleString.h"
#include "SimpleVertex/SimpleVertex.h"
#include "Sprite/Sprite.h"
#include "StringScreen/StringScreen.h"
#include "Timer/Timer.h"
#include "Script.h"

namespace IsoRealms::Basics {
  class Basics : public IModuleHandle {
    public:
    Basics(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals);

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject* project, JSONObject object) override;
    void save(JSONObject object, IAssetIdentifier* identifier) override;
    void registerAssets(IAssetRegistry* assets) override;
    void unregisterAssets(IAssetRemover* remover, IAssets* releaser) override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void reloadGlobalConfiguration();
    void persistGlobalConfiguration();
    float getSoundVolume();
    void setSoundVolume(float volume);
    float getMusicVolume();
    void setMusicVolume(float volume);

    private:
    static const std::string ID_RESOURCE_ANALOGUE_INPUT;
    static const std::string ID_RESOURCE_BOOLEAN_TRIGGER;
    static const std::string ID_RESOURCE_COLOUR_CYCLER;
    static const std::string ID_RESOURCE_COLOUR_SWITCH;
    static const std::string ID_RESOURCE_DIGITAL_INPUT;
    static const std::string ID_RESOURCE_FILE_FONT;
    static const std::string ID_RESOURCE_FILE_SOUND;
    static const std::string ID_RESOURCE_FILE_TEXTURE;
    static const std::string ID_RESOURCE_FLOAT_SWITCH;
    static const std::string ID_RESOURCE_FUNCTION;
    static const std::string ID_RESOURCE_INPUT_GROUP;
    static const std::string ID_RESOURCE_INPUT_SWITCH;
    static const std::string ID_RESOURCE_INTERRUPT_HANDLER;
    static const std::string ID_RESOURCE_MODEL_CYCLER;
    static const std::string ID_RESOURCE_MODEL_SCREEN;
    static const std::string ID_RESOURCE_PROJECT;
    static const std::string ID_RESOURCE_PROJECT_OPTIONS;
    static const std::string ID_RESOURCE_SEQUENCE;
    static const std::string ID_RESOURCE_SIMPLE_BOOLEAN;
    static const std::string ID_RESOURCE_SIMPLE_COLOUR;
    static const std::string ID_RESOURCE_SIMPLE_FLOAT;
    static const std::string ID_RESOURCE_SIMPLE_INTEGER;
    static const std::string ID_RESOURCE_SIMPLE_STRING;
    static const std::string ID_RESOURCE_SIMPLE_VERTEX;
    static const std::string ID_RESOURCE_SPRITE;
    static const std::string ID_RESOURCE_STRING_SCREEN;
    static const std::string ID_RESOURCE_TIMER;

    static const std::string NAME_RESOURCE_ANALOGUE_INPUT;
    static const std::string NAME_RESOURCE_BOOLEAN_TRIGGER;
    static const std::string NAME_RESOURCE_COLOUR_CYCLER;
    static const std::string NAME_RESOURCE_COLOUR_SWITCH;
    static const std::string NAME_RESOURCE_DIGITAL_INPUT;
    static const std::string NAME_RESOURCE_FILE_FONT;
    static const std::string NAME_RESOURCE_FILE_SOUND;
    static const std::string NAME_RESOURCE_FILE_TEXTURE;
    static const std::string NAME_RESOURCE_FLOAT_SWITCH;
    static const std::string NAME_RESOURCE_FUNCTION;
    static const std::string NAME_RESOURCE_INPUT_GROUP;
    static const std::string NAME_RESOURCE_INPUT_SWITCH;
    static const std::string NAME_RESOURCE_INTERRUPT_HANDLER;
    static const std::string NAME_RESOURCE_MODEL_CYCLER;
    static const std::string NAME_RESOURCE_MODEL_SCREEN;
    static const std::string NAME_RESOURCE_PROJECT;
    static const std::string NAME_RESOURCE_PROJECT_OPTIONS;
    static const std::string NAME_RESOURCE_SEQUENCE;
    static const std::string NAME_RESOURCE_SIMPLE_BOOLEAN;
    static const std::string NAME_RESOURCE_SIMPLE_COLOUR;
    static const std::string NAME_RESOURCE_SIMPLE_FLOAT;
    static const std::string NAME_RESOURCE_SIMPLE_INTEGER;
    static const std::string NAME_RESOURCE_SIMPLE_STRING;
    static const std::string NAME_RESOURCE_SIMPLE_VERTEX;
    static const std::string NAME_RESOURCE_SPRITE;
    static const std::string NAME_RESOURCE_STRING_SCREEN;
    static const std::string NAME_RESOURCE_TIMER;
    
    static const std::string ACTION_TYPE_SCRIPT;
    static const std::string SEQUENCE_TRACK_TYPE_ACTION;
    static const std::string SEQUENCE_TRACK_TYPE_COLOUR;

    static const std::string JSON_ANALOGUE_INPUT_MAPPINGS;
    static const std::string JSON_DIGITAL_INPUT_MAPPINGS;
    static const std::string JSON_ID;
    static const std::string JSON_MODULE_SETTINGS;
    static const std::string JSON_MUSIC_VOLUME;
    static const std::string JSON_SOUND_VOLUME;

    static const std::string GLOBAL_CONFIGURATION_FILE;

    ResourceTypeDefinition<Basics, AnalogueInput>    cResourceTypeAnalogueInput;
    ResourceTypeDefinition<Basics, BooleanTrigger>   cResourceTypeBooleanTrigger;
    ResourceTypeDefinition<Basics, ColourCycler>     cResourceTypeColourCycler;
    ResourceTypeDefinition<Basics, ColourSwitch>     cResourceTypeColourSwitch;
    ResourceTypeDefinition<Basics, DigitalInput>     cResourceTypeDigitalInput;
    ResourceTypeDefinition<Basics, FileFont>         cResourceTypeFileFont;
    ResourceTypeDefinition<Basics, FileSound>        cResourceTypeFileSound;
    ResourceTypeDefinition<Basics, FileTexture>      cResourceTypeFileTexture;
    ResourceTypeDefinition<Basics, FloatSwitch>      cResourceTypeFloatSwitch;
    ResourceTypeDefinition<Basics, Function>         cResourceTypeFunction;
    ResourceTypeDefinition<Basics, InputGroup>       cResourceTypeInputGroup;
    ResourceTypeDefinition<Basics, InputSwitch>      cResourceTypeInputSwitch;
    ResourceTypeDefinition<Basics, InterruptHandler> cResourceTypeInterruptHandler;
    ResourceTypeDefinition<Basics, ModelCycler>      cResourceTypeModelCycler;
    ResourceTypeDefinition<Basics, ModelScreen>      cResourceTypeModelScreen;
    ResourceTypeDefinition<Basics, Project>          cResourceTypeProject;
    ResourceTypeDefinition<Basics, ProjectOptions>   cResourceTypeProjectOptions;
    ResourceTypeDefinition<Basics, Sequence>         cResourceTypeSequence;
    ResourceTypeDefinition<Basics, SimpleBoolean>    cResourceTypeSimpleBoolean;
    ResourceTypeDefinition<Basics, SimpleColour>     cResourceTypeSimpleColour;
    ResourceTypeDefinition<Basics, SimpleFloat>      cResourceTypeSimpleFloat;
    ResourceTypeDefinition<Basics, SimpleInteger>    cResourceTypeSimpleInteger;
    ResourceTypeDefinition<Basics, SimpleString>     cResourceTypeSimpleString;
    ResourceTypeDefinition<Basics, SimpleVertex>     cResourceTypeSimpleVertex;
    ResourceTypeDefinition<Basics, Sprite>           cResourceTypeSprite;
    ResourceTypeDefinition<Basics, StringScreen>     cResourceTypeStringScreen;
    ResourceTypeDefinition<Basics, Timer>            cResourceTypeTimer;

    Script cActionTypeScript;
    LuaBinding<Basics> cLuaBinding;

    static float cSoundVolume;
    static float cMusicVolume;
  };
}
