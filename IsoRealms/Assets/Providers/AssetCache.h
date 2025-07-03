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

#include <string>

#include "IsoRealms/Utils.h"

namespace IsoRealms {
  
  // TODO: This used to cache literals of the same value, but since literals
  // became editable/mutable, it can no longer serve this purpose.  Thus for
  // now it just cache multiple instances of literals with the same value.
  // Maybe I can find a way to improve this later.
  template<class OWNER, class TYPE> class AssetCache {
    public:
    TYPE* getCachedAsset(OWNER& owner, const std::string& value) {
      return cCache.emplace_back(this->getUncachedAsset(owner, value)).get();
    }

    TYPE* getCachedAsset(OWNER& owner, JSONObject object) {
      return cCache.emplace_back(this->getUncachedAsset(owner, object)).get();
    }
    
    TYPE* getCachedAsset(OWNER& owner) {
      return cCache.emplace_back(this->getUncachedAsset(owner)).get();
    }

    void releaseCachedAsset(const TYPE* asset) {
      Utils::removeElementUnique(cCache, asset);
    }

    private:
    mutable std::vector<std::unique_ptr<TYPE>> cCache;

    virtual std::unique_ptr<TYPE> getUncachedAsset(OWNER& owner, const std::string& value) = 0;
    virtual std::unique_ptr<TYPE> getUncachedAsset(OWNER& owner, JSONObject object) = 0;
    virtual std::unique_ptr<TYPE> getUncachedAsset(OWNER& owner) = 0;
  };
}
