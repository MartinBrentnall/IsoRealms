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

#include "Modules/Equilibria/Assets/Type/ICamera.h"

namespace IsoRealms::Equilibria {
  class WorldView;

  class CameraOverview final : public ICamera,
                               public IFloat {
    public:
    
    // Constructors.
    CameraOverview(const Metadata& metadata, WorldView& view);
    CameraOverview(const Metadata& metadata, WorldView& view, JSONObject object);

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
    
    // Runtime data.
    float cCachedXLocation;
    float cCachedYLocation;
    float cCachedZLocation;
    float cCachedXZoom;
    float cCachedYZoom;

    // External interfaces.
    WorldView& cParent; /// Parent view.
  };
}
