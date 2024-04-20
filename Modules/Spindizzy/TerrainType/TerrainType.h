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

#include <GL/glew.h>

#include "IsoRealms/Collision/CollisionUtils.h"
#include "IsoRealms/Condition/ConditionElement.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Client/SurfacePattern.h"
#include "Modules/Spindizzy/Assets/Client/WallPattern.h"
#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/WorldEditorCursorCell.h"

namespace IsoRealms::Spindizzy {
  class ISpindizzyRegistry;
  class Spindizzy;
  class Zone;
  
  class TerrainType : public IWorldEditorTool {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    TerrainType(IProject* project, Spindizzy* spindizzy);
    TerrainType(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);
    
    ~TerrainType();
    
    // Interface to be used by module.
    void registerAssets(ISpindizzyRegistry* registry);
    
    
    
    
    
    
    

    ISurfacePattern* getSurfacePattern() const;
    IWallPattern* getWestWallPattern() const;
    IWallPattern* getEastWallPattern() const;
    IWallPattern* getSouthWallPattern() const;
    IWallPattern* getNorthWallPattern() const;
    
    Spindizzy* getSpindizzy() const;
    void executeContactScript();
    void executeImpactScript();
    float getSurfaceFriction() const;
    float getSurfaceGrip() const;
    float getSurfaceBounce() const;
    float getWallBounce() const;
    bool isRespawnAllowed() const;
    bool isSolid() const;

    // Editing.
    std::vector<ConditionElement*> getTerrainStateConditionElements();

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor* editor) override;
    bool renderAssetIcon() const override;

    private:

    // DOM strings.
    static const std::string TAG_IMPACT_ACTION;
    static const std::string TAG_CONTACT_ACTION;
    static const std::string TAG_SURFACE;
    static const std::string TAG_SPLIT_SURFACE_A;
    static const std::string TAG_SPLIT_SURFACE_B;
    static const std::string TAG_WEST_WALL;
    static const std::string TAG_EAST_WALL;
    static const std::string TAG_SOUTH_WALL;
    static const std::string TAG_NORTH_WALL;
    static const std::string TAG_TEXTURE;

    static const std::string ATTRIBUTE_FRICTION;
    static const std::string ATTRIBUTE_GRIP;
    static const std::string ATTRIBUTE_BOUNCE;
    static const std::string ATTRIBUTE_PATTERN;
    static const std::string ATTRIBUTE_RESPAWN_ALLOWED;
    static const std::string ATTRIBUTE_SOLID;
    static const std::string ATTRIBUTE_DEFAULT_ZONE_TYPE;
    static const std::string ATTRIBUTE_DEFAULT_THEME_SET;
    static const std::string ATTRIBUTE_WALL_BOUNCE;

    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(TerrainType& parent, WorldEditor* editor);
      
      /***************************************\
       * Implements IWorldEditorToolInstance *
      \***************************************/
      bool isTool(IWorldEditorTool* tool) const override;
      bool renderIcon(float yaw) const override;
      void renderEditingPreview() const override;
      void renderUI(float aspectRatio) const override;
      void updateUI(unsigned int milliseconds) override;
      bool inputEdit(sf::Event& event, double yaw) override;
      void processCursorMovement(LiteralVertex* start, LiteralVertex* end) override;
      double getSnapInterval() const override;
      
      private:
      TerrainType& cParent;
      WorldEditor* cEditor;
      Zone* cPinnedZone;
      WorldEditorCursorCell cPinnedLocation;
      bool cDrawingNegation;
      bool cDrawingSteppedBase;

      void draw();
      bool cancel();
      void toggleNegation();
      void toggleShapeEditor();
    };

    // Defaults.
    static const float DEFAULT_WALL_BOUNCE;

    Spindizzy* cSpindizzy;
      
    // Properties
    float cDefSurfaceFriction;
    float cDefSurfaceGrip;
    float cDefSurfaceBounce;
    float cDefWallBounce;
    bool cDefRespawnAllowed;
    bool cDefSolid;
    SurfacePattern cDefSurfacePattern;
    WallPattern cDefWestWallPattern;
    WallPattern cDefEastWallPattern;
    WallPattern cDefSouthWallPattern;
    WallPattern cDefNorthWallPattern;
    Action cDefContactAction;
    Action cDefImpactAction;
    
    // Editing data.
    std::vector<std::unique_ptr<Pen>> cEditingPens; /// Pens for drawing terrain of this type.
  };
}
