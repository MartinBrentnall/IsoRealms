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
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "AssetIDException.h"
#include "IAssetListener.h"
#include "IAssetUser.h"

#include "IsoRealms/IAssets.h"

namespace IsoRealms {
  template<class OWNER, class TYPE> class AssetRegistry final {
    public:
    void add(IAssetProvider<OWNER, TYPE>* provider, const std::string& key, const std::string& category) {
      
      // Don't allow nullptr assets to be added
      if (provider == nullptr) {
        throw std::invalid_argument("ERROR: AssetRegistry::add: Specified provider is nullptr.");
      }
      
      // It's OK, register the asset and ID.
      cProviders.emplace(key, std::make_pair(provider, category));
      for (unsigned int i = 0; i < cListeners.size(); i++) {
        cListeners[i]->assetAdded(provider);
      }
    }

    IAssetProvider<OWNER, TYPE>* getProvider(const std::string& id, bool required) {
      typename std::map<std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>>::iterator mValue = cProviders.find(id);
      if (mValue != cProviders.end()) {
        return mValue->second.first;
      } else if (!required) {
        return nullptr;
      }

      // Debug.
      std::cout << this << ": ERROR: AssetRegistry::get: No matching provider for key \"" << id << "\"." << std::dec << std::endl;
      for (const std::pair<const std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>>& mPair : cProviders) {
        std::cout << "Registered Provider: " << mPair.first << std::endl;
      }
      throw AssetIDException("ERROR: AssetRegistry::get: No matching provider for key \"" + id + "\".");
    }

    void remove(IAssetProvider<OWNER, TYPE>* provider) {
      // TODO cProviders.erase(provider);
    }
    
    std::string getID(const IAssetProvider<OWNER, TYPE>* provider) const {
      for (std::pair<std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>> mPair : cProviders) {
        if (provider == mPair.second.first) {
          return mPair.first;
        }
      }
      std::cout << "WARNING: AssetRegistry::getID: Specified provider not found in this registry" << std::endl;
      return "";
    }
    
/*    std::vector<std::pair<std::string, std::string>> getAll() {
      TODO: I think this was used for editing (browsing).
    }*/

    void addAssetListener(IAssetListener<OWNER, TYPE>* assetListener) {
      cListeners.push_back(assetListener);
    }
    
/*    bool renderIcon(const std::string& id) const {
      TODO: I think this was used for editing (browsing).
    }*/

    void checkClean(const std::string& name) {
      if (!cProviders.empty()) {
        std::cout << "WARNING: AssetRegistry::checkClean: " << cProviders.size() << " \"" << name << "\" providers not removed." << std::endl;
        for (std::pair<IAssetProvider<OWNER, TYPE>*, std::string>& mPair : cProviders) {
          std::cout << " - Asset provider info: ";
          mPair.first->info();
        }
      }
    }
    
    private:
    std::map<std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>> cProviders;
    std::vector<IAssetListener<OWNER, TYPE>*> cListeners;
  };
}
