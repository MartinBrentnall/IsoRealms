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

#include <GL/glew.h>

#include "IsoRealms.h"

#include "Modules/Equilibria/Resources/Client/Camera.h"
#include "Modules/Equilibria/Resources/Client/ZoneViewType.h"
#include "Modules/Equilibria/Resources/Type/ICamera.h"
#include "Modules/Equilibria/Resources/Type/IZoneView.h"
#include "Modules/Equilibria/Resources/Type/IZoneViewType.h"
#include "Modules/Equilibria/EquilibriaResourceRegistry.h"
#include "Modules/Equilibria/World/World.h"

namespace IsoRealms::Equilibria {
  class Equilibria;

  /**
   * Component definition for a view of a Equilibria world.
   */
  class WorldView final : public IScreen {
    public:
    
    /***********************\
     * Component Interface *
    \***********************/
    WorldView(Equilibria& equilibria, IComponentData& data);
    void define(IComponentDefiner& definer);
    void publish(ResourcePublisher& publisher);
    void publish(const std::string& parentID);

    /******************************\
     * Resource client interfaces *
    \******************************/
    Equilibria& getResourceManager();
    Project& getProject() const;
    bool isReadOnly() const;
    void setOwner(ProjectFile* owner);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
    void reset();

    // Interface to be used by Equilibria.
    void addZoneView(Zone* zone);
    void removeZoneView(Zone* zone);
    World* getWorld() const;
    Equilibria& getEquilibria();
    IComponentData& getComponentData();
    ICamera* getCamera();

    /***********************\
     * Scripting Interface *
    \***********************/
    void setZone(Zone* zone);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    const IFloat* getYaw() const override;
    const IFloat* getPitch() const override;

    private:

    class ZoneView {
      public:
      ZoneView(Zone* zone, std::unique_ptr<IZoneView> view);
      
      Zone* cZone;
      std::unique_ptr<IZoneView> cView;      
    };

    inline static const float DEFAULT_ZOOM  = 1.0f;

    inline static const std::string TYPE_ZONE_VIEW = "ZoneView";
    
    // External interfaces.
    Equilibria& cEquilibria;      /// Equilibria module reference.
    IComponentData& cComponentData;

    // Definition data.
    ComponentReference<World, WorldView> cDefWorld; /// World being viewed.
    Camera cDefCamera;                              /// Camera for this view.
    ZoneViewType cDefZoneViewType;                  /// The type of representation of zones within this view.
    float cDefZoom = DEFAULT_ZOOM;                  /// Zoom adjustment.
    
    // Runtime data.
    std::vector<std::unique_ptr<ZoneView>> cRuntimeZoneViews; /// Representation of zones within this view.
    Zone* cRuntimeZone = nullptr;                             /// Zone to be rendered in play mode.

    LuaBinding<WorldView> cLuaBinding;
  };
}
