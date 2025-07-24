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
#include <set>

#include "IsoRealms.h"

#include "Modules/Spindizzy/Assets/Type/IBoundaryType.h"
#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/ISpindizzyRegistry.h"
#include "Modules/Spindizzy/WorldEditorCursorCell.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class World;
  
  /**
   * Resource definition for a type of zone.  The appearance of zones as
   * depicted by the world overview is configurable, and pick-up instances
   * provide boundaries for interaction with movable objects (e.g. players).
   */
  class ZoneType : public IBoundaryType,
                   public IWorldEditorTool {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    ZoneType(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    ZoneType(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner);

    // Destructor.
    virtual ~ZoneType();

    // Interface to be used by module.
    void registerAssets(ISpindizzyRegistry* registry);
    void registerZoneProperty(const std::string& id, IBinding* property);
    void unregisterZoneProperty(const std::string& id);
    
    /****************************\
     * Implements IBoundaryType *
    \****************************/
    std::string getBoundaryTypeID() const override;
    IBinding* getBinding(const std::string& id) const override;
    std::string getBindingID(const IBinding* binding) const override;

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor, IResourceData& owner) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(ZoneType& parent, WorldEditor& editor);
      
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
      ZoneType& cParent;
      WorldEditor& cEditor;
      bool cDrawing;                         /// True when a location has been pinned to start drawing a zone.
      WorldEditorCursorCell cPinnedLocation; /// Pinned start location for drawing a zone.
    };

    // External interfaces.
    Spindizzy& cSpindizzy;   /// Spindizzy module reference.
    
    // Action parameters.
//     LuaBinding<Zone> cRuntimeParameterZone;           /// Parameter for a zone itself.
    std::map<std::string, IBinding*> cZoneProperties; /// Parameters for externally defined properties of a zone.
    
    // Editing data.
    std::vector<std::unique_ptr<Pen>> cEditingPens; /// Pens for drawing zones of this type.
  };
}
