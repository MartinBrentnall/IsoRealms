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
#include "Resources/Fixed/AnalogueInput/DigitalToAnalogueMapping.h"
#include "Resources/Registry/SequenceTrackRegistry.h"
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
    Basics(IsoRealms::Project& project, IComponentTypeRegistry& registry);

    void refreshResourceRegistration(Sequence& sequence);

    const Metadata& getMetadata(const std::string& key) const;

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void publish(ResourcePublisher& publisher) override;
    void updateInputs(unsigned int milliseconds) override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;

    IsoRealms::Project& getProject() const;
    unsigned int getAvailableFunctionID() const;

    template <typename TYPE> void release(IResourceUser<TYPE>* user, TYPE* resource) {
      cSequenceTracks.release(user, resource);
    }

    template <typename TYPE> TreeItemInfo getTreeItemInfo(const TYPE* resource) const {
      return cSequenceTracks.getTreeItemInfo(resource);
    }

    template <typename TYPE> const Metadata& getPropertyMetadata(const TYPE* resource) const {
      return cSequenceTracks.getPropertyMetadata(resource);
    }

    template <typename TYPE> void save(JSONObject object, const TYPE* resource) const {
      cSequenceTracks.save(object, resource);
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

    template <typename TYPE, typename OWNER> TYPE* createDefault(IResourceUser<TYPE>* user, OWNER& owner) {
      return cSequenceTracks.getDefault(user, owner);
    }

    template <typename TYPE, typename OWNER> TYPE* getResource(IResourceUser<TYPE>* user, const std::string& id, OWNER& owner, IStateListener* listener = nullptr) {
      return cSequenceTracks.get(user, owner, id, listener);
    }

    template <typename TYPE, typename OWNER> TYPE* getResource(IResourceUser<TYPE>* user, JSONObject object, OWNER& owner, IStateListener* listener = nullptr, bool required = true) {
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
    IComponentTypeRegistry& cModule;
    
    Script cActionScript;

    // Resource registries
    SequenceTrackRegistry cSequenceTracks;

    ComponentTypeDefinition<Basics, AnalogueControl>   cComponentTypeAnalogueControl;
    ComponentTypeDefinition<Basics, BooleanTrigger>    cComponentTypeBooleanTrigger;
    ComponentTypeDefinition<Basics, DigitalControl>    cComponentTypeDigitalControl;
    ComponentTypeDefinition<Basics, FileFont>          cComponentTypeFileFont;
    ComponentTypeDefinition<Basics, FileSound>         cComponentTypeFileSound;
    ComponentTypeDefinition<Basics, FileTexture>       cComponentTypeFileTexture;
    ComponentTypeDefinition<Basics, Function>          cComponentTypeFunction;
    ComponentTypeDefinition<Basics, InputGroup>        cComponentTypeInputGroup;
    ComponentTypeDefinition<Basics, InputSwitch>       cComponentTypeInputSwitch;
    ComponentTypeDefinition<Basics, InterruptHandler>  cComponentTypeInterruptHandler;
    ComponentTypeDefinition<Basics, Project>           cComponentTypeProject;
    ComponentTypeDefinition<Basics, ProjectConfigurer> cComponentTypeProjectConfigurer;
    ComponentTypeDefinition<Basics, Sequence>          cComponentTypeSequence;
    ComponentTypeDefinition<Basics, SimpleBoolean>     cComponentTypeSimpleBoolean;
    ComponentTypeDefinition<Basics, SimpleColour>      cComponentTypeSimpleColour;
    ComponentTypeDefinition<Basics, SimpleFloat>       cComponentTypeSimpleFloat;
    ComponentTypeDefinition<Basics, SimpleInteger>     cComponentTypeSimpleInteger;
    ComponentTypeDefinition<Basics, SimpleString>      cComponentTypeSimpleString;
    ComponentTypeDefinition<Basics, SimpleVertex>      cComponentTypeSimpleVertex;
    ComponentTypeDefinition<Basics, Sprite>            cComponentTypeSprite;

    ResourceInstanced<IComponentData, IAnalogueInput, DigitalToAnalogueMapping> cDigitalToAnalogueMapping;

    LuaBinding<Basics> cLuaBinding;

    static float cSoundVolume;
    static float cMusicVolume;
  };
}
