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
#pragma once

#include "IsoRealms.h"

#include "AnalogueControl/AnalogueControl.h"
#include "Assets/Fixed/AnalogueInput/DigitalToAnalogueMapping.h"
#include "Assets/Registry/SequenceTrackRegistry.h"
#include "BooleanTrigger/BooleanTrigger.h"
#include "DigitalControl/DigitalControl.h"
#include "FileFont/FileFont.h"
#include "FileSound/FileSound.h"
#include "FileTexture/FileTexture.h"
#include "Function/Function.h"
#include "InputGroup/InputGroup.h"
#include "InputSwitch/InputSwitch.h"
#include "InterruptHandler/InterruptHandler.h"
#include "Project/Project.h"
#include "ProjectConfigurer/ProjectConfigurer.h"
#include "Sequence/Sequence.h"
#include "SimpleBoolean/SimpleBoolean.h"
#include "SimpleColour/SimpleColour.h"
#include "SimpleFloat/SimpleFloat.h"
#include "SimpleInteger/SimpleInteger.h"
#include "SimpleString/SimpleString.h"
#include "SimpleVertex/SimpleVertex.h"
#include "Sprite/Sprite.h"
#include "Script.h"

namespace IsoRealms::Basics {
  class Basics : public IModuleHandle {
    public:
    Basics(IsoRealms::Project& project, IResourceTypeRegistry& registry);

    void refreshAssetRegistration(Sequence& sequence);

    const Metadata& getMetadata(const std::string& key) const;

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void registerAssets(ResourceAssetRegistry& assets) override;
    void updateInputs(unsigned int milliseconds) override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;

    IsoRealms::Project& getProject() const;
    unsigned int getAvailableFunctionID() const;

    template <typename TYPE> void release(IAssetUser<TYPE>* user, TYPE* asset) {
      cSequenceTracks.release(user, asset);
    }

    template <typename TYPE> TreeItemInfo getTreeItemInfo(const TYPE* asset) const {
      return cSequenceTracks.getTreeItemInfo(asset);
    }

    template <typename TYPE> const Metadata& getPropertyMetadata(const TYPE* asset) const {
      return cSequenceTracks.getPropertyMetadata(asset);
    }

    template <typename TYPE> void save(JSONObject object, const TYPE* asset) const {
      cSequenceTracks.save(object, asset);
    }

    template <typename TYPE> void forEachEntry(const std::function<void(const TreeItemInfo&)>& f) const {
      cSequenceTracks.forEachEntry(f);
    }

    template <typename TYPE> bool renderIcon(const std::string& id) const {
      return cSequenceTracks.renderIcon(id);
    }

    template <typename TYPE> bool isConfigurable(const std::string& id) const {
      return cSequenceTracks.hasConfiguration(id);
    }

    template <typename TYPE, typename OWNER> TYPE* createDefault(IAssetUser<TYPE>* user, OWNER& owner) {
      return cSequenceTracks.getDefault(user, owner);
    }

    template <typename TYPE, typename OWNER> TYPE* getAsset(IAssetUser<TYPE>* user, const std::string& id, OWNER& owner, IStateListener* listener = nullptr) {
      return cSequenceTracks.get(user, owner, id, listener);
    }

    template <typename TYPE, typename OWNER> TYPE* getAsset(IAssetUser<TYPE>* user, JSONObject object, OWNER& owner, IStateListener* listener = nullptr, bool required = true) {
      return cSequenceTracks.get(user, owner, object, listener, required);
    }

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
    inline static const std::string ACTION_SCRIPT = "Script";

    inline static const std::string JSON_ANALOGUE_INPUT_MAPPINGS = "analogueInputMappings";
    inline static const std::string JSON_DIGITAL_INPUT_MAPPINGS  = "digitalInputMappings";
    inline static const std::string JSON_ID                      = "id";
    inline static const std::string JSON_MODULE_SETTINGS         = "moduleSettings";
    inline static const std::string JSON_MUSIC_VOLUME            = "musicVolume";
    inline static const std::string JSON_SOUND_VOLUME            = "soundVolume";

    inline static const std::string GLOBAL_CONFIGURATION_FILE = "Module_IsoRealms.json";

    // External interfaces.
    IsoRealms::Project& cProject;
    IResourceTypeRegistry& cModule;
    
    Script cActionScript;

    // Asset registries
    SequenceTrackRegistry cSequenceTracks;

    ResourceTypeDefinition<Basics, AnalogueControl>   cResourceTypeAnalogueControl;
    ResourceTypeDefinition<Basics, BooleanTrigger>    cResourceTypeBooleanTrigger;
    ResourceTypeDefinition<Basics, DigitalControl>    cResourceTypeDigitalControl;
    ResourceTypeDefinition<Basics, FileFont>          cResourceTypeFileFont;
    ResourceTypeDefinition<Basics, FileSound>         cResourceTypeFileSound;
    ResourceTypeDefinition<Basics, FileTexture>       cResourceTypeFileTexture;
    ResourceTypeDefinition<Basics, Function>          cResourceTypeFunction;
    ResourceTypeDefinition<Basics, InputGroup>        cResourceTypeInputGroup;
    ResourceTypeDefinition<Basics, InputSwitch>       cResourceTypeInputSwitch;
    ResourceTypeDefinition<Basics, InterruptHandler>  cResourceTypeInterruptHandler;
    ResourceTypeDefinition<Basics, Project>           cResourceTypeProject;
    ResourceTypeDefinition<Basics, ProjectConfigurer> cResourceTypeProjectConfigurer;
    ResourceTypeDefinition<Basics, Sequence>          cResourceTypeSequence;
    ResourceTypeDefinition<Basics, SimpleBoolean>     cResourceTypeSimpleBoolean;
    ResourceTypeDefinition<Basics, SimpleColour>      cResourceTypeSimpleColour;
    ResourceTypeDefinition<Basics, SimpleFloat>       cResourceTypeSimpleFloat;
    ResourceTypeDefinition<Basics, SimpleInteger>     cResourceTypeSimpleInteger;
    ResourceTypeDefinition<Basics, SimpleString>      cResourceTypeSimpleString;
    ResourceTypeDefinition<Basics, SimpleVertex>      cResourceTypeSimpleVertex;
    ResourceTypeDefinition<Basics, Sprite>            cResourceTypeSprite;

    AssetInstanced<IResourceData, IAnalogueInput, DigitalToAnalogueMapping> cDigitalToAnalogueMapping;

    LuaBinding<Basics> cLuaBinding;

    static float cSoundVolume;
    static float cMusicVolume;
  };
}
