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

#include <functional>

#include "IsoRealms.h"

#include "Modules/Spindizzy/Assets/Type/ISurfacePattern.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class TerrainType;
  
  class SurfacePattern : public Asset<SurfacePattern, ISurfacePattern, TerrainType>,
                         public IStateListener<ISurfacePattern*> {
    public:
    SurfacePattern(Spindizzy& spindizzy, TerrainType& owner, std::function<void()> listener);

    /**************************************************\
     * Implements Asset<ISurfacePattern, TerrainType> *
    \**************************************************/
    ISurfacePattern* createLiteralAsset(TerrainType& owner);
    ISurfacePattern* getAsset(TerrainType& owner, JSONObject object);
    ISurfacePattern* getAsset(TerrainType& owner, const std::string& id);
    bool isDefaultConfiguration() const;

    /***********************************************\
     * Implements IStateListener<ISurfacePattern*> *
    \***********************************************/
    void stateChanged(ISurfacePattern* asset) override;

    private:
    std::function<void()> cListener;
  };
}
