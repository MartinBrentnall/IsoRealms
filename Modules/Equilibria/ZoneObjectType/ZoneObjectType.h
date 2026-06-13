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

#include <cmath>

#include "IsoRealms.h"

#include "Modules/Equilibria/Assets/Type/IWorldEditorTool.h"
#include "Modules/Equilibria/Assets/Type/IZoneObjectTypeTrait.h"
#include "Modules/Equilibria/EquilibriaAssetRegistry.h"

#include "IZoneObjectTraitEditor.h"

namespace IsoRealms::Equilibria {
  class Equilibria;
  class Zone;

  class ZoneObjectType : public IWorldEditorTool {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ZoneObjectType(Equilibria& equilibria, IResourceData& data);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    bool hasReadOnlyReferences() const;
    void overrideReadOnlyReferences();

    ~ZoneObjectType();
    
    // Interface to be used by module.
    void registerAssets(const std::string& parentID);

    // Interface to be used by trait types.
    Equilibria& getEquilibria();
    IResourceData& getResourceData();
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
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor, IResourceData& owner) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    inline static const std::string JSON_EDITING_PROCESSOR = "editingProcessor";
    inline static const std::string JSON_EDITING_RENDERER  = "editingRenderer";
    inline static const std::string JSON_ID                = "id";
    inline static const std::string JSON_RUNTIME_PROCESSOR = "runtimeProcessor";
    inline static const std::string JSON_RUNTIME_RENDERER  = "runtimeRenderer";
    inline static const std::string JSON_TRAITS            = "traits";
    inline static const std::string JSON_TYPE              = "type";

    inline static const std::string BIND_TO_TRAIT = "Trait";
    inline static const std::string BIND_TO_ZONE  = "Zone";

    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(ZoneObjectType& parent, WorldEditor& editor);
      
      /***************************************\
       * Implements IWorldEditorToolInstance *
      \***************************************/
      bool isTool(IWorldEditorTool* tool) const override;
      bool renderIcon(float yaw) const override;
      void renderEditingPreview() const override;
      void renderUI(float aspectRatio) const override;
      void updateUI(unsigned int milliseconds) override;
      bool inputTool(sf::Event& event) override;
      bool inputTool(SignalInputID id, double yaw) override;
      bool isCursorLocked() const override;
      void processCursorMovement(LiteralVertex* start, LiteralVertex* end) override;
      double getSnapInterval() const override;
      
      private:
      ZoneObjectType& cParent;
      WorldEditor& cEditor;
      int cEditingTrait;
      Zone* cPinnedZone;
    };

    // External interfaces.
    Equilibria& cEquilibria; /// Equilibria module reference.
    IResourceData& cResourceData;

    // Asset registry.
    EquilibriaAssetRegistry cAssets; /// Equilibria asset registry.

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
