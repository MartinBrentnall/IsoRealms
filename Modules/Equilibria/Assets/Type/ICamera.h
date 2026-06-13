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

namespace IsoRealms::Equilibria {
  class ICameraListener;
  class Zone;

  class ICamera : public IAsset {
    public:
    virtual void registerAssets(ComponentAssetRegistry& assets, const std::string& parentID) = 0;
    virtual void updateRuntime(unsigned int milliseconds) = 0;
    virtual void reset() = 0;
    virtual const IFloat* getYaw() const = 0;
    virtual const IFloat* getPitch() const = 0;
    virtual float getXLocation() const = 0;
    virtual float getYLocation() const = 0;
    virtual float getZLocation() const = 0;
    virtual float getXZoom() const = 0;
    virtual float getYZoom() const = 0;
    virtual void setZone(Zone* zone) = 0;
    virtual void addListener(ICameraListener* listener) = 0;
    virtual void removeListener(ICameraListener* listener) = 0;

    virtual ~ICamera() {
    }
  };
}
