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

#include <memory>
#include <vector>

#include "IsoRealms/Persistence.h"

namespace IsoRealms {
  class IProperty;
  class IPropertyOwner;
  class IResourceData;

  /**
   * Base interface for assets.
   */
  class IAsset {
    public:
    virtual bool renderAssetIcon() const = 0;
    virtual void saveAsset(JSONObject object) const = 0;
    virtual std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) = 0;
    virtual bool isDefaultConfiguration() const = 0;

    virtual ~IAsset() {}
  };
}

