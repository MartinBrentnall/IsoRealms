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

#include "IsoRealms/Assets/Providers/AssetInstanced.h"
#include "IsoRealms/IModuleHandle.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceTypeRegistry.h"
#include "IsoRealms/IsoRealmsConstants.h"
#include "IsoRealms/ResourceTypeDefinition.h"

#include "AnalogueInput/AnalogueInput.h"
#include "Assets/Fixed/SequenceTrack/SequenceTrackAction.h"
#include "Assets/Fixed/SequenceTrack/SequenceTrackAudio.h"
#include "Assets/Fixed/SequenceTrack/SequenceTrackColour.h"
#include "Assets/Fixed/SequenceTrack/SequenceTrackFloat.h"
#include "Assets/Fixed/SequenceTrack/SequenceTrackDummy.h"
#include "Assets/Type/ISequenceTrack.h"
#include "BooleanTrigger/BooleanTrigger.h"
#include "ColourCycler/ColourCycler.h"
#include "DigitalInput/DigitalInput.h"
#include "FileFont/FileFont.h"
#include "FileSound/FileSound.h"
#include "FileTexture/FileTexture.h"
#include "Function/Function.h"
#include "IBasics.h"
#include "InputGroup/InputGroup.h"
#include "InputSwitch/InputSwitch.h"
#include "InterruptHandler/InterruptHandler.h"
#include "ModelCycler/ModelCycler.h"
#include "Project/Project.h"
#include "ProjectConfigurer/ProjectConfigurer.h"
#include "ProjectOptions/ProjectOptions.h"
#include "Sequence/Sequence.h"
#include "SimpleBoolean/SimpleBoolean.h"
#include "SimpleColour/SimpleColour.h"
#include "SimpleFloat/SimpleFloat.h"
#include "SimpleInteger/SimpleInteger.h"
#include "SimpleString/SimpleString.h"
#include "SimpleVertex/SimpleVertex.h"
#include "Sprite/Sprite.h"
#include "Timer/Timer.h"
#include "Script.h"

namespace IsoRealms::Basics {
  class Basics : public IModuleHandle,
                 public IBasics {
    public:
    Basics(IProject& project, IResourceTypeRegistry* registry);

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject& project, JSONObject object) override;
    void save(JSONObject object, IAssetIdentifier& identifier) override;
    void registerAssets(IAssetRegistry& assets) override;
    void unregisterAssets(IAssetRemover& remover, IAssets& releaser) override;
    std::vector<std::unique_ptr<IProperty>> getProperties() override;

    /**********************\
     * Implements IBasics *
    \**********************/
    IProject& getProject() const override;
    std::vector<std::string> getAllSequenceTracks() override;
    std::string getID(const ISequenceTrack* asset) const override;
    bool renderSequenceTrackIcon(const std::string& id) const override;
    bool isSequenceTrackConfigurable(const std::string& id) const override;
    ISequenceTrack* createLiteralSequenceTrack(IAssetUser<ISequenceTrack>* user, Sequence& owner) override;
    ISequenceTrack* getSequenceTrack(IAssetUser<ISequenceTrack>* user, JSONObject object, Sequence& owner) override;
    ISequenceTrack* getSequenceTrack(IAssetUser<ISequenceTrack>* user, const std::string& id, Sequence& owner) override;
    void release(IAssetUser<ISequenceTrack>* user, ISequenceTrack* asset) override;
    void save(JSONObject object, ISequenceTrack* asset) const override;

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
    static const std::string ID_RESOURCE_PROJECT_CONFIGURER;
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

    static const std::string SEQUENCE_TRACK_ACTION;
    static const std::string SEQUENCE_TRACK_AUDIO;
    static const std::string SEQUENCE_TRACK_COLOUR;
    static const std::string SEQUENCE_TRACK_FLOAT;

    static const std::string ACTION_TYPE_SCRIPT;

    static const std::string JSON_ANALOGUE_INPUT_MAPPINGS;
    static const std::string JSON_DIGITAL_INPUT_MAPPINGS;
    static const std::string JSON_ID;
    static const std::string JSON_MODULE_SETTINGS;
    static const std::string JSON_MUSIC_VOLUME;
    static const std::string JSON_SOUND_VOLUME;

    static const std::string GLOBAL_CONFIGURATION_FILE;

    // External interfaces.
    IProject& cProject;
    
    // Asset registries
    AssetClientManager<Sequence, ISequenceTrack> cSequenceTracks;
    
    // Dummy asset providers.
    AssetLiteralDummy<Sequence, ISequenceTrack, SequenceTrackDummy> cDummyProviderSequenceTrack;
    
    // Built-in providers for UI asset types.
    AssetInstanced<Sequence, ISequenceTrack, SequenceTrackAction> cProviderSequenceTrackAction;
    AssetInstanced<Sequence, ISequenceTrack, SequenceTrackAudio>  cProviderSequenceTrackAudio;
    AssetInstanced<Sequence, ISequenceTrack, SequenceTrackColour> cProviderSequenceTrackColour;
    AssetInstanced<Sequence, ISequenceTrack, SequenceTrackFloat>  cProviderSequenceTrackFloat;

    ResourceTypeDefinition<Basics, AnalogueInput>     cResourceTypeAnalogueInput;
    ResourceTypeDefinition<Basics, BooleanTrigger>    cResourceTypeBooleanTrigger;
    ResourceTypeDefinition<Basics, ColourCycler>      cResourceTypeColourCycler;
    ResourceTypeDefinition<Basics, DigitalInput>      cResourceTypeDigitalInput;
    ResourceTypeDefinition<Basics, FileFont>          cResourceTypeFileFont;
    ResourceTypeDefinition<Basics, FileSound>         cResourceTypeFileSound;
    ResourceTypeDefinition<Basics, FileTexture>       cResourceTypeFileTexture;
    ResourceTypeDefinition<Basics, Function>          cResourceTypeFunction;
    ResourceTypeDefinition<Basics, InputGroup>        cResourceTypeInputGroup;
    ResourceTypeDefinition<Basics, InputSwitch>       cResourceTypeInputSwitch;
    ResourceTypeDefinition<Basics, InterruptHandler>  cResourceTypeInterruptHandler;
    ResourceTypeDefinition<Basics, ModelCycler>       cResourceTypeModelCycler;
    ResourceTypeDefinition<Basics, Project>           cResourceTypeProject;
    ResourceTypeDefinition<Basics, ProjectConfigurer> cResourceTypeProjectConfigurer;
    ResourceTypeDefinition<Basics, ProjectOptions>    cResourceTypeProjectOptions;
    ResourceTypeDefinition<Basics, Sequence>          cResourceTypeSequence;
    ResourceTypeDefinition<Basics, SimpleBoolean>     cResourceTypeSimpleBoolean;
    ResourceTypeDefinition<Basics, SimpleColour>      cResourceTypeSimpleColour;
    ResourceTypeDefinition<Basics, SimpleFloat>       cResourceTypeSimpleFloat;
    ResourceTypeDefinition<Basics, SimpleInteger>     cResourceTypeSimpleInteger;
    ResourceTypeDefinition<Basics, SimpleString>      cResourceTypeSimpleString;
    ResourceTypeDefinition<Basics, SimpleVertex>      cResourceTypeSimpleVertex;
    ResourceTypeDefinition<Basics, Sprite>            cResourceTypeSprite;
    ResourceTypeDefinition<Basics, Timer>             cResourceTypeTimer;

    Script cActionTypeScript;
    LuaBinding<Basics> cLuaBinding;

    static float cSoundVolume;
    static float cMusicVolume;
  };
}
