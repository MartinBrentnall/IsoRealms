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

#include <GL/glew.h>

#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Client/Camera.h"
#include "Modules/Spindizzy/Assets/Client/ZoneViewType.h"
#include "Modules/Spindizzy/Assets/Type/ICamera.h"
#include "Modules/Spindizzy/Assets/Type/IZoneView.h"
#include "Modules/Spindizzy/Assets/Type/IZoneViewType.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a view of a Spindizzy world.
   */
  class WorldView final : public IScreen {
    public:
    
    /**********************\
     * Resource Interface *
    \**********************/
    WorldView(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    WorldView(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);

    // Interface to be used by Spindizzy.
    void registerAssets(ISpindizzyRegistry* registry);
    void addZoneView(Zone* zone);
    void removeZoneView(Zone* zone);
    World* getWorld() const;
    Spindizzy& getSpindizzy();
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

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:

    class Pitch : public IFloat {
      public:
      Pitch(WorldView& parent);

      /*********************\
       * Implements IFloat *
      \*********************/
      float getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;

      private:
      WorldView& cDefParent;
    };
    
    class ZoneView {
      public:
      ZoneView(Zone* zone, std::unique_ptr<IZoneView> view);
      
      Zone* cZone;
      std::unique_ptr<IZoneView> cView;      
    };

    // JSON members.
    static const std::string JSON_CAMERA;
    static const std::string JSON_TYPE;
    static const std::string JSON_WORLD;
    static const std::string JSON_ZOOM;

    static const std::string TYPE_ZONE_VIEW;
    
    // External interfaces.
    Spindizzy& cSpindizzy;      /// Spindizzy module reference.

    // Definition data.
    World* cDefWorld;              /// World being viewed.
    Camera cDefCamera;             /// Camera for this view.
    ZoneViewType cDefZoneViewType; /// The type of representation of zones within this view.
    float cDefZoom;                /// Zoom adjustment.
    
    // Runtime data.
    std::vector<std::unique_ptr<ZoneView>> cRuntimeZoneViews; /// Representation of zones within this view.
    Zone* cRuntimeZone;                                       /// Zone to be rendered in play mode.

    LuaBinding<WorldView> cLuaBinding;
  };
}
