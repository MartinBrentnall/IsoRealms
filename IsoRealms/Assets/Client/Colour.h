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

#include "IsoRealms/Assets/Type/IColour.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/IStateListener.h"

#include "Asset.h"

namespace IsoRealms {
  class Colour : public Asset<Colour, IColour, IResourceData>,
                 public IStateListener<IColour*> {
    public:
    Colour(IResourceData& owner, float defaultRed = 0.0f, float defaultGreen = 0.0f, float defaultBlue = 0.0f, float defaultAlpha = 1.0f, std::function<void()> listener = nullptr);

    /****************************************************\
     * Implements Asset<Colour, IColour, IResourceData> *
    \****************************************************/
    IColour* createDefaultAsset(IResourceData& owner);

    /***************************************\
     * Implements IStateListener<IColour*> *
    \***************************************/
    void stateChanged(IColour* asset) override;

    private:
    float cDefaultRed;
    float cDefaultGreen;
    float cDefaultBlue;
    float cDefaultAlpha;
    std::function<void()> cListener;
  };
}
