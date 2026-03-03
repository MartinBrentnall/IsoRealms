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
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "AssetIDException.h"
#include "TreeItemInfo.h"
#include "IAssetListener.h"
#include "IAssetUser.h"

namespace IsoRealms {
  template <typename OWNER, typename TYPE> class AssetRegistry final {
    public:
    void add(IAssetProvider<OWNER, TYPE>* provider, const std::string& key, const std::string& category) {

      // Don't allow nullptr assets to be added
      if (provider == nullptr) {
        throw std::invalid_argument("ERROR: AssetRegistry::add: Specified provider is nullptr.");
      }

      // Check if it's already registered and just rename it if so.
      for (typename std::map<std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>>::iterator it = cProviders.begin(); it != cProviders.end(); ++it) {
        if (it->second.first == provider) {
          typename std::map<std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>>::node_type node = cProviders.extract(it);
          node.key() = key;
          cProviders.insert(std::move(node));
          return;
        }
      }

      // It's not already registered, so register the asset and ID.
      cProviders.emplace(key, std::make_pair(provider, category));
      for (unsigned int i = 0; i < cListeners.size(); i++) {
        cListeners[i]->assetAdded(provider);
      }
    }

    IAssetProvider<OWNER, TYPE>* getProvider(const std::string& id, bool required) const {
      typename std::map<std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>>::const_iterator mValue = cProviders.find(id);
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
      for (std::pair<const std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>>& mPair : cProviders) {
        if (mPair.second.first == provider) {
          cProviders.erase(mPair.first);
          break;
        }
      }
    }
    
    TreeItemInfo getTreeItemInfo(const IAssetProvider<OWNER, TYPE>* provider) const {
      for (const std::pair<const std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>>& mPair : cProviders) {
        if (provider == mPair.second.first) {
          return TreeItemInfo{mPair.first, mPair.second.second};
        }
      }
      std::cout << "WARNING: AssetRegistry::getTreeItemInfo: Specified provider not found in this registry" << std::endl;
      return TreeItemInfo{"", ""};
    }

    void forEachEntry(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
      for (const std::pair<const std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>>& p : cProviders) {
        getTreeItemInfoFunction(TreeItemInfo{p.first, p.second.second});
      }
    }

    void addAssetListener(IAssetListener<OWNER, TYPE>* assetListener) {
      cListeners.push_back(assetListener);
    }
    
    bool renderIcon(const std::string& id) const {
      IAssetProvider<OWNER, TYPE>* mProvider = getProvider(id, true);
      return mProvider->renderAssetProviderIcon();
    }

    void checkClean(const std::string& name) {
      if (!cProviders.empty()) {
        std::cout << "WARNING: AssetRegistry::checkClean: " << cProviders.size() << " \"" << name << "\" providers not removed." << std::endl;
        for (std::pair<std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>> mPair : cProviders) {
          std::cout << " - \"" << mPair.first << std::endl;
        }
      }
    }
    
    private:
    std::map<std::string, std::pair<IAssetProvider<OWNER, TYPE>*, std::string>> cProviders;
    std::vector<IAssetListener<OWNER, TYPE>*> cListeners;
  };
}
