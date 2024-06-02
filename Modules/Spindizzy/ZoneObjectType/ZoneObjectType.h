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

#include <cmath>

#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/Assets/Type/IZoneObjectTypeTrait.h"

#include "IZoneObjectTraitEditor.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class Zone;

  class ZoneObjectType : public IWorldEditorTool {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ZoneObjectType(IProject* project, Spindizzy* spindizzy);
    ZoneObjectType(IProject* project, Spindizzy* spindizzy, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    ~ZoneObjectType();
    
    // Interface to be used by module.
    void registerAssets(ISpindizzyRegistry* registry);

    // Interface to be used by trait types.
    Spindizzy& getSpindizzy();
    void registerEditor(IZoneObjectTraitEditor* editor);
    IBinding* getBinding(const std::string& id) const;
    std::string getBindingID(const IBinding* binding) const;

    // Interface to be used by objects.
    std::map<std::string, std::unique_ptr<IZoneObjectTrait>> createTraits(ZoneObject& object) const;
    std::string getID(const IZoneObjectTypeTrait* trait) const;
    std::string getRuntimeRendererID() const;
    std::string getEditingRendererID() const;
    std::string getRuntimeProcessorID() const;
    std::string getEditingProcessorID() const;

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor* editor) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;

    private:

    // JSON members.
    static const std::string JSON_EDITING_PROCESSOR;
    static const std::string JSON_EDITING_RENDERER;
    static const std::string JSON_ID;
    static const std::string JSON_RUNTIME_PROCESSOR;
    static const std::string JSON_RUNTIME_RENDERER;
    static const std::string JSON_TRAITS;
    static const std::string JSON_TYPE;

    static const std::string BIND_TO_TRAIT;
    static const std::string BIND_TO_ZONE;

    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(ZoneObjectType& parent, WorldEditor* editor);
      
      /***************************************\
       * Implements IWorldEditorToolInstance *
      \***************************************/
      bool isTool(IWorldEditorTool* tool) const override;
      bool renderIcon(float yaw) const override;
      void renderEditingPreview() const override;
      void renderUI(float aspectRatio) const override;
      void updateUI(unsigned int milliseconds) override;
      bool inputTool(SignalInputID id, double yaw) override;
      bool isCursorLocked() const override;
      void processCursorMovement(LiteralVertex* start, LiteralVertex* end) override;
      double getSnapInterval() const override;
      
      private:
      ZoneObjectType& cParent;
      WorldEditor* cEditor;
      int cEditingTrait;
      Zone* cPinnedZone;
    };

    // External interfaces.
    Spindizzy& cDefSpindizzy; /// Spindizzy module reference.
    
    // Definition data.
    std::map<std::string, IZoneObjectTypeTrait*> cDefTypeTraits; /// Traits of this object type.
    std::string cDefRuntimeRendererID;
    std::string cDefEditingRendererID;
    std::string cDefRuntimeProcessorID;
    std::string cDefEditingProcessorID;
    
    // Runtime data.
    std::vector<IZoneObjectTraitEditor*> cRuntimeEditors;

    // Editing data.
    std::vector<std::unique_ptr<Pen>> cEditingPens; /// Pens for drawing objects of this type.
    
    // Private functions.
    IBinding* getTraitBinding(const std::string& id) const; /// Retrieves a binding from a trait of this object type.
  };
}
