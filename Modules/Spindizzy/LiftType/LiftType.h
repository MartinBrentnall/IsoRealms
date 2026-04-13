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
#include <GL/glew.h>
#include <optional>

#include "IsoRealms.h"

#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/SpindizzyAssetRegistry.h"
#include "Modules/Spindizzy/WorldEditorCursorCell.h"

namespace IsoRealms::Spindizzy {
  class ISpindizzyRegistry;
  class Spindizzy;
  class Zone;

  /**
   * Resource definition for a type of lift.  The appearance, active state, and
   * action to perform when an instance of this lift type moves are
   * configurable.
   */
  class LiftType final : public IWorldEditorTool {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    LiftType(Spindizzy& spindizzy, IResourceData& data);
    LiftType(Spindizzy& spindizzy, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    bool hasReadOnlyReferences() const;
    void overrideReadOnlyReferences();

    // Interface to be used by module.    
    void registerAssets(const std::string& parentID);
    
    // Interface to be used by lift instances.
    bool isActive();
    void executeTickAction();
    std::unique_ptr<ModelInstance> createModel();
    
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
    static const std::string JSON_APPEARANCE;
    static const std::string JSON_ON_TICK;
    static const std::string JSON_STATE;

    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(LiftType& parent, WorldEditor& editor);
      
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
      LiftType& cParent;
      WorldEditor& cEditor;
      Zone* cPinnedZone;                     /// Zone in which drawing a lift of this type is currently taking place.
      WorldEditorCursorCell cPinnedLocation; /// Pinned initial location for a lift being drawn.
      std::optional<int> cPinnedRange;       /// Pinned movement range for a lift being drawn.
    
      // Private functions.    
      void renderArrowLines(float rangeA, float rangeB) const;
      void draw();
      bool cancel();
    };

    // External interfaces.
    Spindizzy& cSpindizzy; /// Spindizzy module reference.
    
    // Asset registry.
    SpindizzyAssetRegistry cAssets; /// Spindizzy asset registry.

    // Definition data.
    Model cDefModel;          /// Visual representation of this lift type.
    Boolean cDefActive;       /// Lifts of this type are active when true.
    Action cDefTickAction;    /// Action to execute at each whole unit of movement of a lift of this type.

    // Editing data.
    std::vector<std::unique_ptr<Pen>> cEditingPens; /// Pens for drawing lifts of this type.
  };
}
