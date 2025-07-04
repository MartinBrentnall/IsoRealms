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
#include <queue>

#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Client/Camera.h"
#include "Modules/Spindizzy/Assets/Type/ICameraListener.h"

namespace IsoRealms::Spindizzy {
  class WorldView;

  class CameraTransitional final : public ICamera,
                                   public ICameraListener {
    public:
    
    // Constructors.
    CameraTransitional(IProject& project, WorldView& view);
    CameraTransitional(IProject& project, WorldView& view, JSONObject object);

    /***********************\
     * Scripting interface *
    \***********************/
    void transitionToStart();
    void transitionToEnd();
    
    /**********************\
     * Implements ICamera *
    \**********************/
    void registerAssets(IAssetRegistry& assets) override;
    void updateRuntime(unsigned int milliseconds) override;
    void reset() override;
    const IFloat* getYaw() const override;
    const IFloat* getPitch() const override;
    float getXLocation() const override;
    float getYLocation() const override;
    float getZLocation() const override;
    float getXZoom() const override;
    float getYZoom() const override;
    void setZone(Zone* zone) override;
    void addListener(ICameraListener* listener) override;
    void removeListener(ICameraListener* listener) override;
    
    /*********************************\
     * Implements IAsset via ICamera *
    \*********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    /******************************\
     * Implements ICameraListener *
    \******************************/
    void yawChanged(ICamera* camera) override;
    void pitchChanged(ICamera* camera) override;


    private:
    
    // JSON members.
    static const std::string JSON_DURATION;
    static const std::string JSON_END;
    static const std::string JSON_ON_END_ARRIVAL;
    static const std::string JSON_ON_END_DEPARTURE;
    static const std::string JSON_ON_START_ARRIVAL;
    static const std::string JSON_ON_START_DEPARTURE;
    static const std::string JSON_START;
    
    static const unsigned int DEFAULT_DURATION;

    class Yaw : public IFloat {
      public:
      Yaw(CameraTransitional& parent);
      
      /*********************\
       * Implements IFloat *
      \*********************/
      float getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      CameraTransitional& cParent;
    };
    
    class Pitch : public IFloat {
      public:
      Pitch(CameraTransitional& parent);
      
      /*********************\
       * Implements IFloat *
      \*********************/
      float getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      CameraTransitional& cParent;
    };

    class Transition : public IFloat {
      public:
      Transition(CameraTransitional& parent);

      /*********************\
       * Implements IFloat *
      \*********************/
      float getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      CameraTransitional& cParent;
    };

    // External interfaces.
    WorldView& cParent; /// Parent view.
    Yaw cYaw;
    Pitch cPitch;
    Transition cTransition;

    // Definition data.
    Camera cDefStart;              /// Camera to start transition at.
    Camera cDefEnd;                /// Camera to end transition at.
    unsigned int cDefDuration;       /// Duration of transition animation between overview and play mode.
    Action cDefStartDepartureAction; /// Action to execute when animation departs from the start camera.
    Action cDefStartArrivalAction;   /// Action to execute when animation arrives at the start camera.
    Action cDefEndDepartureAction;   /// Action to execute when animation departs from the end camera.
    Action cDefEndArrivalAction;     /// Action to execute when animation arrives at the end camera.
    
    // Runtime data.
    bool cRuntimeEnd;                                 /// True when animating towards (or at) end camera, false when animating towards (or at) start camera.
    unsigned int cRuntimeAnimation;                   /// Animates movement between start and end camera.
    IStateNotifier<IFloat>* cRuntimeYawStateNotifier; /// Anything interested in knowing when the Yaw changes will be notified via this.

    // Scripting support.
    LuaBinding<CameraTransitional> cLuaBinding; /// Allow transitional cameras to be bound to lua variables.
  };
}
