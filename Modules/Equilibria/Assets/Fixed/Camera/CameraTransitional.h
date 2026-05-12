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
#include <queue>

#include "IsoRealms.h"

#include "Modules/Equilibria/Assets/Client/Camera.h"
#include "Modules/Equilibria/Assets/Type/ICamera.h"
#include "Modules/Equilibria/Assets/Type/ICameraListener.h"

namespace IsoRealms::Equilibria {
  class WorldView;

  class CameraTransitional final : public ICamera,
                                   public ICameraListener {
    public:
    
    // Constructors.
    CameraTransitional(const Metadata& metadata, WorldView& view);
    CameraTransitional(const Metadata& metadata, WorldView& view, JSONObject object);

    /***********************\
     * Scripting interface *
    \***********************/
    void transitionToStart();
    void transitionToEnd();
    
    /**********************\
     * Implements ICamera *
    \**********************/
    void registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) override;
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
    void getAssetProperties(IPropertyMaker& owner) override;
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
      void getAssetProperties(IPropertyMaker& owner) override;
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
      void getAssetProperties(IPropertyMaker& owner) override;
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
      void getAssetProperties(IPropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      CameraTransitional& cParent;
    };

    // External interfaces.
    const Metadata& cMetadata;
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
    bool cRuntimeEnd;                         /// True when animating towards (or at) end camera, false when animating towards (or at) start camera.
    unsigned int cRuntimeAnimation;           /// Animates movement between start and end camera.
    IStateNotifier* cRuntimeYawStateNotifier; /// Anything interested in knowing when the Yaw changes will be notified via this.

    // Scripting support.
    LuaBinding<CameraTransitional> cLuaBinding; /// Allow transitional cameras to be bound to lua variables.
  };
}
