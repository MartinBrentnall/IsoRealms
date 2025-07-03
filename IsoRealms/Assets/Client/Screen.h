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

#include <functional>

#include "IsoRealms/Assets/Type/IScreen.h"
#include "IsoRealms/IResourceData.h"

#include "Asset.h"

namespace IsoRealms {
  class Screen : public Asset<Screen, IScreen, IResourceData> {
    public:
    Screen(IResourceData& owner);
    void render(float scale, float aspectRatio) const;

    /********************************************\
     * Implements Asset<IScreen, IResourceData> *
    \********************************************/
    IScreen* createLiteralAsset(IResourceData& owner);
    IScreen* getAsset(IResourceData& owner, JSONObject object);
    IScreen* getAsset(IResourceData& owner, const std::string& id);
    std::vector<std::string> getAvailableProviders() const;
    bool renderOtherProviderIcon(const std::string& id) const;
    bool hasConfiguration() const;
    bool isDefaultConfiguration() const;
  };
}
