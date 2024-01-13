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

#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Type/ICamera.h"

namespace IsoRealms::Spindizzy {
  class WorldView;

  class CameraDummy final : public ICamera,
                            public IFloat {
    public:
    
    /**********************\
     * Implements ICamera *
    \**********************/
    void registerAssets(IAssetRegistry* assets) override; 
    void unregisterAssets(IAssetRemover* assets) override;
    void save(DOMNodeWriter* node) const override;
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
    bool renderAssetIcon() const override;
  };
}
