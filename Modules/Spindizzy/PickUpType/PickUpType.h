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

#include <memory>
#include <cmath>

#include "IsoRealms.h"

#include "Modules/Spindizzy/Assets/Type/IBoundaryType.h"
#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"

namespace IsoRealms::Spindizzy {
  class ISpindizzyRegistry;
  class Spindizzy;

  /**
   * Resource definition for a type of pick up.  The appearance of pick-ups of
   * this type is configurable, and pick-up instances provide boundaries for
   * interaction with movable objects (e.g. players).
   */
  class PickUpType final : public IBoundaryType,
                           public IWorldEditorTool {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    PickUpType(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    PickUpType(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner);

    ~PickUpType();

    // Interface to be used by module.
    void registerAssets(ISpindizzyRegistry* registry);
    
    std::unique_ptr<ModelInstance> createModel();
    
    /****************************\
     * Implements IBoundaryType *
    \****************************/
    std::string getBoundaryTypeID() const override;
    IBinding* getBinding(const std::string& id) const override;
    std::string getBindingID(const IBinding* binding) const override;

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_APPEARANCE;

    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(PickUpType& parent, WorldEditor& editor);
      
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
      PickUpType& cParent;
      WorldEditor& cEditor;
    };

    // External interfaces.
    Spindizzy& cSpindizzy; /// Spindizzy module reference.
    
    // Definition data.
    Model cDefModel;   /// Visual representation of this pick up type.

    // Editing data.
    std::vector<std::unique_ptr<Pen>> cEditingPens; /// Pens for drawing pick-ups of this type.
  };
}
