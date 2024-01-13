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

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Persistence/DOMNode.h"

namespace IsoRealms {
  template<class TYPE> class AssetCache {
    public:
    TYPE* getCachedAsset(const std::string& value) const {
      std::string mExpression = normalize(value);
      typename std::map<std::string, std::pair<std::unique_ptr<TYPE>, unsigned int>>::iterator mIterator = cCache.find(mExpression);
      if (mIterator != cCache.end()) {
        mIterator->second.second++;
        return mIterator->second.first.get();
      }
      std::unique_ptr<TYPE> mAsset = this->getUncachedAsset(value);
      return cCache.emplace(mExpression, std::make_pair(std::move(mAsset), 1)).first->second.first.get();
    }
    
    void releaseCachedAsset(const TYPE* asset) {
      for (std::pair<const std::string, std::pair<std::unique_ptr<TYPE>, unsigned int>>& mPair : cCache) {
        if (mPair.second.first.get() == asset) {
          mPair.second.second--;
          if (mPair.second.second == 0) {
            cCache.erase(mPair.first);
          }
          break;
        }
      }
    }

    void cacheInfo() const {
      std::cout << "Asset cache with " << cCache.size() << " cached assets:" << std::endl;
      for (std::pair<const std::string, std::pair<std::unique_ptr<TYPE>, unsigned int>>& mPair : cCache) {
        std::cout << "    - Cached asset: \"" << mPair.first << "\" (clients: " << mPair.second.second << ")" << std::endl;
      }
    }

    private:
    inline static const std::string ATTRIBUTE_VALUE = "value";

    mutable std::map<std::string, std::pair<std::unique_ptr<TYPE>, unsigned int>> cCache;
    
    virtual std::string normalize(const std::string& expression) const = 0;
    virtual std::unique_ptr<TYPE> getUncachedAsset(const std::string& value) const = 0;
  };
}
