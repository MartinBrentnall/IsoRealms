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

#include "Modules/Spindizzy/Assets/Type/ICamera.h"

namespace IsoRealms::Spindizzy {
  class WorldView;
  class Zone;

  class CameraGameplay final : public ICamera,
                               public IFloat {
    public:
    
    // Constructors.
    CameraGameplay(const Metadata& metadata, WorldView& view);
    CameraGameplay(const Metadata& metadata, WorldView& view, JSONObject object);

    /***********************\
     * Scripting interface *
    \***********************/
    void faceNorthWest();
    void faceNorthEast();
    void faceSouthWest();
    void faceSouthEast();
    void rotateLeft();
    void rotateRight();
    void flip();
    
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
    
    /*********************\
     * Implements IFloat *
    \*********************/
    float getValue() const override;

    /****************************************\
     * Implements IAsset via ICamera/IFloat *
    \****************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_DIRECTION;
    static const std::string JSON_ROTATE_DURATION;

    static const std::string DIRECTION_NORTH_EAST;
    static const std::string DIRECTION_NORTH_WEST;
    static const std::string DIRECTION_SOUTH_EAST;
    static const std::string DIRECTION_SOUTH_WEST;
    static const std::string DIRECTION_INVALID;

    // Constants.
    static const int DEFAULT_DURATION;
    static const int VALUE_MAX;
    static const int VALUE_MIN;
    static const int VALUE_NORTH_EAST;
    static const int VALUE_NORTH_WEST;
    static const int VALUE_SOUTH_EAST;
    static const int VALUE_SOUTH_WEST;
    static const int VALUE_INVALID;
    

    // External interfaces.
    const Metadata& cMetadata;
    WorldView& cParent; /// Parent view.

    LiteralFloat cPitch;

    // Definition data.
    float cDefAngle;          /// Initial angle value.
    int cDefRollDuration;     /// Millisecond duration of transition to a new angle.

    // Runtime data.
    float cRuntimePreviousValue;   /// Angle from which view is rotating.
    float cRuntimeCurrentValue;    /// Angle to which view is rotating.
    int cRuntimeRollTimeRemaining; /// Milliseconds remaining of transition to a new angle.
    float cCachedXLocation;
    float cCachedYLocation;
    float cCachedZLocation;
    float cCachedXZoom;
    float cCachedYZoom;
    ICameraListener* cListener;

    LuaBinding<CameraGameplay> cLuaBinding;
    IStateNotifier* cStateNotifier;
    
    // Private functions.
    void rollTo(float value);
    std::string getDirectionString() const;
    static int getDirectionValue(const std::string& value);
  };
}
