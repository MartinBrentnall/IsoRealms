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

#include "IsoRealms/Assets/Providers/AssetSingleton.h"
#include "IsoRealms/Assets/Type/IStateNotifier.h"
#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Persistence/JSONObject.h"

#include "AssetRegistry.h"

namespace IsoRealms {
  template <typename DERIVED, typename OWNER, typename TYPE> class AssetClientManager {
    public:
    inline static const std::string JSON_KEY = "key";

    AssetRegistry<OWNER, TYPE> cRegistry;

    AssetClientManager() :
              AssetClientManager(static_cast<IAssetProvider<OWNER, TYPE>*>(nullptr)) {
    }

    AssetClientManager(TYPE* defaultAsset, const std::string& defaultProviderID = "None", const std::string& defaultProviderName = "None") {
      cDefaultProvider = cAssetSingletons.emplace(defaultAsset, std::make_unique<AssetSingleton<OWNER, TYPE>>(defaultProviderID, defaultAsset)).first->second.get();
      add(cDefaultProvider, defaultProviderID, defaultProviderName, false);
    }

    AssetClientManager(IAssetProvider<OWNER, TYPE>* defaultProvider, const std::string& defaultProviderID = "None", const std::string& defaultProviderName = "None") :
              cDefaultProvider(defaultProvider) {
      if (cDefaultProvider != nullptr) {
        add(cDefaultProvider, defaultProviderID, defaultProviderName);
      }
    }

    IStateNotifier* add(TYPE* asset, const std::string& id, const std::string& category = "TODO", bool stateChanges = false) {
      typename std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>>::iterator mIterator = cAssetSingletons.find(asset);
      if (mIterator == cAssetSingletons.end()) {
        return add(cAssetSingletons.emplace(asset, std::make_unique<AssetSingleton<OWNER, TYPE>>(id, asset)).first->second.get(), id, category, stateChanges);
      }
      return add(mIterator->second.get(), id, category);
    }
    
    IStateNotifier* add(IAssetProvider<OWNER, TYPE>* provider, const std::string& id, const std::string& category, bool stateChanges = false) {
      cRegistry.add(provider, id, category);
      if (stateChanges) {
        typename std::map<const IAssetProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(provider);
        if (mNotifier != cStateNotifiers.end()) {
          return mNotifier->second.get();
        }
        return cStateNotifiers.emplace(provider, std::make_unique<StateNotifier>()).first->second.get();
      }
      return nullptr;
    }
    
    void remove(TYPE* asset) {
      typename std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>>::iterator mIterator = cAssetSingletons.find(asset);
      if (mIterator == cAssetSingletons.end()) {
        std::cout << "WARNING: AssetClientManager::remove: Specified asset has no singleton in this registry." << std::endl;
      } else {
        remove(mIterator->second.get());
        cAssetSingletons.erase(asset);
//        cRegistry.checkClean("DEBUG");
      }
    }

    void remove(IAssetProvider<OWNER, TYPE>* provider) {
      typename std::map<IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>>::iterator mIterator = cClients.find(provider);
      if (mIterator != cClients.end()) {
        for (std::pair<TYPE*, std::vector<IAssetUser<TYPE>*>> mPair : mIterator->second) {
          for (IAssetUser<TYPE>* mClient : mPair.second) {
            mClient->relinquish(mPair.first);
          }
        }
        cClients.erase(provider);
      }
      cStateNotifiers.erase(provider);
      cRegistry.remove(provider);
    }
    
    bool hasReadOnlyReferences(TYPE* asset) const {
      typename std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>>::const_iterator mIterator = cAssetSingletons.find(asset);
      if (mIterator == cAssetSingletons.end()) {
        std::cout << "WARNING: AssetClientManager::hasReadOnlyReferences: Specified asset has no singleton in this registry." << std::endl;
      } else {
        return hasReadOnlyReferences(mIterator->second.get());
      }
      return false;
    }
    
    bool hasReadOnlyReferences(IAssetProvider<OWNER, TYPE>* provider) const {
      typename std::map<IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>>::const_iterator mIterator = cClients.find(provider);
      if (mIterator != cClients.end()) {
        for (std::pair<TYPE*, std::vector<IAssetUser<TYPE>*>> mPair : mIterator->second) {
          for (IAssetUser<TYPE>* mClient : mPair.second) {
            if (mClient->isReadOnly()) {
              return true;
            }
          }
        }
      }
      return false;
    }
    
    void overrideReadOnlyReferences(TYPE* asset, ProjectFile* owner) {
      typename std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>>::iterator mIterator = cAssetSingletons.find(asset);
      if (mIterator == cAssetSingletons.end()) {
        std::cout << "WARNING: AssetClientManager::overrideReadOnlyReferences: Specified asset has no singleton in this registry." << std::endl;
      } else {
        return overrideReadOnlyReferences(mIterator->second.get(), owner);
      }
    }
    
    void overrideReadOnlyReferences(IAssetProvider<OWNER, TYPE>* provider, ProjectFile* owner) {
      typename std::map<IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>>::iterator mIterator = cClients.find(provider);
      if (mIterator != cClients.end()) {
        for (std::pair<TYPE*, std::vector<IAssetUser<TYPE>*>> mPair : mIterator->second) {
          for (IAssetUser<TYPE>* mClient : mPair.second) {
            if (mClient->isReadOnly()) {
              mClient->setOwner(owner);
            }
          }
        }
      }
    }
    
    TYPE* getDefault(IAssetUser<TYPE>* client, OWNER& owner) {
      TYPE* mAsset = cDefaultProvider != nullptr ? cDefaultProvider->getAsset(owner) : nullptr;
      if (mAsset != nullptr) {
        cClients[cDefaultProvider][mAsset].emplace_back(client);
      }
      return mAsset;
    }

    TYPE* get(IAssetUser<TYPE>* client, OWNER& owner, JSONObject object, IStateListener* listener, bool required) {
      if (client == nullptr) {
        throw std::invalid_argument("Client cannot be null");
      }

      TYPE* mAsset = nullptr;
      IAssetProvider<OWNER, TYPE>* mProvider = cRegistry.getProvider(object.getString(JSON_KEY), required);
      if (mProvider != nullptr) {
        mAsset = mProvider->getAsset(owner, object);
      }
      registerClient(client, mProvider, mAsset, listener);
      return mAsset;
    }

    void registerClient(IAssetUser<TYPE>* client, IAssetProvider<OWNER, TYPE>* provider, TYPE* asset, IStateListener* listener = nullptr) {
      if (asset != nullptr) {
        cClients[provider][asset].emplace_back(client);

        if (listener != nullptr) {
          typename std::map<const IAssetProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(provider);
          if (mNotifier != cStateNotifiers.end()) {
            mNotifier->second->addListener(listener);
          }
        }
      }
    }

    TYPE* get(IAssetUser<TYPE>* client, OWNER& owner, const std::string& id, IStateListener* listener) {
      if (client == nullptr) {
        throw std::invalid_argument("Client cannot be null");
      }

      IAssetProvider<OWNER, TYPE>* mProvider = cRegistry.getProvider(id, true);
      TYPE* mAsset = mProvider->getAsset(owner);
      registerClient(client, mProvider, mAsset, listener);
      return mAsset;
    }

    void addStateChangeListener(const TYPE* asset, IStateListener* listener) {
      typename std::map<const IAssetProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(getProvider(asset));
      if (mNotifier != cStateNotifiers.end()) {
        mNotifier->second->addListener(listener);
      }
    }
    
    virtual void forEachEntry(const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const {
      cRegistry.forEachEntry(getTreeItemInfoFunction);
    }
    
    void release(IAssetUser<TYPE>* client, TYPE* asset) {
      if (client == nullptr) {
        std::cout << "WARNING: Client from which to release cannot is nullptr" << std::endl;
        return;
      }
      if (asset == nullptr) {
        std::cout << "WARNING: Asset to release is nullptr" << std::endl;
        return;
      }
      IAssetProvider<OWNER, TYPE>* mProvider = getProvider(asset);
      typename std::map<IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>>::iterator mIterator = cClients.find(mProvider);
      if (mIterator == cClients.end()) {
        std::cout << "WARNING: Specified asset isn't registered with specified client" << std::endl;
        return;
      }
      
      typename std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>::iterator mAssetClientsIterator = mIterator->second.find(asset);
      unsigned int mClientIndex = std::find(mAssetClientsIterator->second.begin(), mAssetClientsIterator->second.end(), client) - mAssetClientsIterator->second.begin();
      if (mClientIndex == mAssetClientsIterator->second.size()) {
        std::cout << "WARNING: Specified client isn't registered as a user of the specified asset" << std::endl;
        return;
      }

      mAssetClientsIterator->second.erase(mAssetClientsIterator->second.begin() + mClientIndex);
      if (mAssetClientsIterator->second.empty()) {
        mIterator->second.erase(asset);
        if (mIterator->second.empty()) {
          cClients.erase(mProvider);
        }
      }

      mProvider->releaseAsset(asset);
      // TODO: Remove state listener (if there is one) from state notifier
    }
    
    TreeItemInfo getTreeItemInfo(const TYPE* asset) const {
      return cRegistry.getTreeItemInfo(getProvider(asset));
    }

    void save(JSONObject object, const TYPE* asset) const {
      const IAssetProvider<OWNER, TYPE>* mProvider = getProvider(asset);
      std::string mID = cRegistry.getTreeItemInfo(mProvider).cID;
      object.addString(JSON_KEY, mID);
      asset->saveAsset(object);
    }

    void addAssetListener(IAssetListener<OWNER, TYPE>* listener) {
      cRegistry.addAssetListener(listener);
    }
    
    virtual bool renderIcon(const std::string& id) const {
      return cRegistry.renderIcon(id);
    }
    
    bool hasConfiguration(const std::string& id) const {
      return cRegistry.getProvider(id, true)->hasConfiguration();
    }
    
    AssetRegistry<OWNER, TYPE>* getRegistry() {
      return &cRegistry;
    }

    void checkClean(const std::string& name) {
      cRegistry.checkClean(name);
      if (!cClients.empty()) {
        std::cout << "WARNING: AssetClientManager::checkClean: " << cClients.size() << " " << name << " providers are still in use." << std::endl;
      }
      if (!cStateNotifiers.empty()) {
        std::cout << "WARNING: AssetClientManager::checkClean: " << cStateNotifiers.size() << " " << name << " have not been removed." << std::endl;
      }
    }
    
    ~AssetClientManager() {
      cDefaultProvider = nullptr;
      for (std::pair<const TYPE* const, std::unique_ptr<AssetSingleton<OWNER, TYPE>>>& mPair : cAssetSingletons) {
        remove(mPair.second.get());
      }
      for (std::pair<const IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>> mPairA : cClients) {
        for (std::pair<TYPE*, std::vector<IAssetUser<TYPE>*>> mPairB : mPairA.second) {
          for (IAssetUser<TYPE>* mClient : mPairB.second) {
            mClient->relinquish(mPairB.first);
          }
        }
      }
    }

    private:
    class StateNotifier final : public IStateNotifier {
      private:
      std::vector<IStateListener*> cListeners;
      
      public:
      void addListener(IStateListener* listener) {
        cListeners.push_back(listener);
      }
      
      /***********************************\
       * Implements IStateNotifier<TYPE> *
      \***********************************/
      void stateChanged() override {
        for (IStateListener* mListener : cListeners) {
          mListener->stateChanged();
        }
      }
    };

    IAssetProvider<OWNER, TYPE>* getProvider(const TYPE* asset) const {
      for (std::pair<IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>> mPairA : cClients) {
        for (std::pair<TYPE*, std::vector<IAssetUser<TYPE>*>> mPairB : mPairA.second) {
          if (mPairB.first == asset) {
            return mPairA.first;
          }
        }
      }

      // Special case where asset is a singleton that has no clients yet.
      typename std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>>::const_iterator mIterator = cAssetSingletons.find(asset);
      if (mIterator != cAssetSingletons.end()) {
        return mIterator->second.get();
      }
      return nullptr;
    }

//     class ProviderData {
//       private:
//       std::optional<StateNotifier> cStateNotifier;
//       std::map<TYPE*, IAssetUser<TYPE*>> cClients;
//     };
    
    std::map<const IAssetProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>> cStateNotifiers;
    std::map<IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>> cClients;
    std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>> cAssetSingletons;
    IAssetProvider<OWNER, TYPE>* cDefaultProvider;
  };
}
