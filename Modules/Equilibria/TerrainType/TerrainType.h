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

#include "IsoRealms.h"

#include "Modules/Equilibria/Resources/Client/SurfacePattern.h"
#include "Modules/Equilibria/Resources/Client/WallPattern.h"
#include "Modules/Equilibria/Resources/Type/IWorldEditorTool.h"
#include "Modules/Equilibria/EquilibriaResourceRegistry.h"
#include "Modules/Equilibria/WorldEditorCursorCell.h"

namespace IsoRealms::Equilibria {
  class IEquilibriaRegistry;
  class Equilibria;
  class Zone;
  
  class TerrainType : public IWorldEditorTool {
    public:
    
    /***********************\
     * Component interface *
    \***********************/
    TerrainType(Equilibria& equilibria, IComponentData& data);
    void define(IComponentDefiner& definer);
    void publish(const std::string& parentID);
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void removed();

    bool hasReadOnlyReferences() const;
    void overrideReadOnlyReferences();

    bool isReadOnly() const; // TODO: Probably shouldn't be here.
    void setOwner(ProjectFile* owner); // TODO: Probably shouldn't be here.

    ISurfacePattern* getSurfacePattern() const;
    IWallPattern* getWestWallPattern() const;
    IWallPattern* getEastWallPattern() const;
    IWallPattern* getSouthWallPattern() const;
    IWallPattern* getNorthWallPattern() const;

    Project& getProject();
    Equilibria& getEquilibria() const;
    Equilibria& getResourceManager();
    IComponentData& getComponentData();
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
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor, IComponentData& owner) override;
    bool renderResourceIcon() const override;
    
    private:
    
    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(TerrainType& parent, WorldEditor& editor);
      
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
      TerrainType& cParent;
      WorldEditor& cEditor;
      Zone* cPinnedZone = nullptr;
      WorldEditorCursorCell cPinnedLocation;
      bool cDrawingNegation = false;
      bool cDrawingSteppedBase;

      void draw();
      bool cancel();
      void toggleNegation();
      void toggleShapeEditor();
    };

    // Defaults.
    inline static const float DEFAULT_WALL_BOUNCE = 0.6f;

    // External interfaces.
    Equilibria& cEquilibria;
    IComponentData& cComponentData;

    // Resource registry.
    EquilibriaResourceRegistry cResources; /// Equilibria resource registry.

    // Properties
    float cDefSurfaceFriction = 0.0f;
    float cDefSurfaceGrip     = 0.0f;
    float cDefSurfaceBounce   = 0.0f;
    float cDefWallBounce      = DEFAULT_WALL_BOUNCE;
    bool cDefRespawnAllowed   = true;
    bool cDefSolid            = false;
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
