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

#include "IsoRealms/Assets/Type/IInteger.h"
#include "IsoRealms/IResourceData.h"

#include "Asset.h"

namespace IsoRealms {
  class Integer : public Asset<Integer, IInteger, IResourceData> {
    public:
    Integer(IResourceData& owner);

    /*********************************************\
     * Implements Asset<IInteger, IResourceData> *
    \*********************************************/
    IInteger* createLiteralAsset(IResourceData& owner);
    IInteger* getAsset(IResourceData& owner, JSONObject object);
    IInteger* getAsset(IResourceData& owner, const std::string& id);
    bool isDefaultConfiguration() const;
  };
}
