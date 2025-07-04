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

#include "IsoRealms/Assets/Providers/AssetSingleton.h"
#include "IsoRealms/Assets/Registry/ILiteralAssetProvider.h"
#include "IsoRealms/Assets/Type/IStateNotifier.h"
#include "IsoRealms/Persistence.h"

#include "AssetRegistry.h"

namespace IsoRealms {
  template<class OWNER, class TYPE> class AssetClientManager {
    public:
    AssetRegistry<OWNER, TYPE> cRegistry;

    AssetClientManager() :
              AssetClientManager(nullptr) {
    }

    AssetClientManager(ILiteralAssetProvider<OWNER, TYPE>* provider, const std::string& label = "None") :
              cLiteralProvider(provider) {
      if (cLiteralProvider != nullptr) {
        add(provider, label, "Literals");
      }
    }

    IStateNotifier<TYPE>* add(TYPE* asset, const std::string& id, const std::string& category = "TODO", bool stateChanges = false) {
      typename std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>>::iterator mIterator = cAssetSingletons.find(asset);
      if (mIterator == cAssetSingletons.end()) {
        return add(cAssetSingletons.emplace(asset, std::make_unique<AssetSingleton<OWNER, TYPE>>(id, asset)).first->second.get(), id, category, stateChanges);
      }
      return add(mIterator->second.get(), id, category);
    }
    
    IStateNotifier<TYPE>* add(IAssetProvider<OWNER, TYPE>* provider, const std::string& id, const std::string& category, bool stateChanges = false) {
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
      typename std::map<const IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>>::iterator mIterator = cClients.find(provider);
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
      typename std::map<const IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>>::const_iterator mIterator = cClients.find(provider);
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
    
    void overrideReadOnlyReferences(TYPE* asset, File* owner) {
      typename std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>>::iterator mIterator = cAssetSingletons.find(asset);
      if (mIterator == cAssetSingletons.end()) {
        std::cout << "WARNING: AssetClientManager::overrideReadOnlyReferences: Specified asset has no singleton in this registry." << std::endl;
      } else {
        return overrideReadOnlyReferences(mIterator->second.get(), owner);
      }
    }
    
    void overrideReadOnlyReferences(IAssetProvider<OWNER, TYPE>* provider, File* owner) {
      typename std::map<const IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>>::iterator mIterator = cClients.find(provider);
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
    
    TYPE* literal(IAssetUser<TYPE>* client, OWNER& owner, const std::string& value) {
      TYPE* mAsset = cLiteralProvider != nullptr ? cLiteralProvider->getLiteralAsset(owner, value) : nullptr;
      if (mAsset != nullptr) {
        cClients[cLiteralProvider][mAsset].emplace_back(client);
      }
      return mAsset;
    }

    TYPE* get(IAssetUser<TYPE>* client, OWNER& owner, JSONObject object, IStateListener<TYPE*>* listener, bool required, std::function<TYPE*(JSONObject object, IStateListener<TYPE*>* listener)> override) {
      if (client == nullptr) {
        throw std::invalid_argument("Client cannot be null");
      }

      TYPE* mAsset = override(object, listener);
      if (mAsset == nullptr) {
        IAssetProvider<OWNER, TYPE>* mProvider = cRegistry.getProvider(object.getString(JSON_KEY), required);
        if (mProvider != nullptr) {
          mAsset = mProvider->getAsset(owner, object);
        }

        if (mAsset != nullptr) {
          cClients[mProvider][mAsset].emplace_back(client);

          if (listener != nullptr) {
            typename std::map<const IAssetProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(mProvider);
            if (mNotifier != cStateNotifiers.end()) {
              mNotifier->second->addListener(listener);
            }
          }
        }
      }
      return mAsset;
    }

    TYPE* get(IAssetUser<TYPE>* client, OWNER& owner, const std::string& id, IStateListener<TYPE*>* listener) {
      if (client == nullptr) {
        throw std::invalid_argument("Client cannot be null");
      }

      IAssetProvider<OWNER, TYPE>* mProvider = cRegistry.getProvider(id, true);
      TYPE* mAsset = mProvider->getAsset(owner);
      if (mAsset != nullptr) {
        cClients[mProvider][mAsset].emplace_back(client);

        if (listener != nullptr) {
          typename std::map<const IAssetProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(mProvider);
          if (mNotifier != cStateNotifiers.end()) {
            mNotifier->second->addListener(listener);
          }
        }
      }
      return mAsset;
    }

    void addStateChangeListener(const TYPE* asset, IStateListener<TYPE*>* listener) {
      typename std::map<const IAssetProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(getProvider(asset));
      if (mNotifier != cStateNotifiers.end()) {
        mNotifier->second->addListener(listener);
      }
    }
    
    std::vector<std::string> getAll() const {
      return cRegistry.getAll();
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
      const IAssetProvider<OWNER, TYPE>* mProvider = getProvider(asset);
      typename std::map<const IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>>::iterator mIterator = cClients.find(mProvider);
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
      // TODO: Remove state listener (if there is one) from state notifier
    }
    
    std::string getID(const TYPE* asset) const {
      return cRegistry.getID(getProvider(asset));
    }

    void save(JSONObject object, const TYPE* asset) const {
      const IAssetProvider<OWNER, TYPE>* mProvider = getProvider(asset);
      std::string mID = cRegistry.getID(mProvider);
      object.addString(JSON_KEY, mID);
      asset->saveAsset(object);
    }

    void addAssetListener(IAssetListener<OWNER, TYPE>* listener) {
      cRegistry.addAssetListener(listener);
    }
    
    bool renderIcon(const std::string& id) const {
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
      cLiteralProvider = nullptr;
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
    inline static const std::string JSON_KEY = "key";

    class StateNotifier final : public IStateNotifier<TYPE> {
      private:
      std::vector<IStateListener<TYPE*>*> cListeners;
      
      public:
      void addListener(IStateListener<TYPE*>* listener) {
        cListeners.push_back(listener);
      }
      
      /***********************************\
       * Implements IStateNotifier<TYPE> *
      \***********************************/
      void stateChanged(TYPE* asset) override {
        for (IStateListener<TYPE*>* mListener : cListeners) {
          mListener->stateChanged(asset);
        }
      }
    };

    const IAssetProvider<OWNER, TYPE>* getProvider(const TYPE* asset) const {
      for (std::pair<const IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>> mPairA : cClients) {
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
    std::map<const IAssetProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IAssetUser<TYPE>*>>> cClients;
    std::map<const TYPE*, std::unique_ptr<AssetSingleton<OWNER, TYPE>>> cAssetSingletons;
    ILiteralAssetProvider<OWNER, TYPE>* cLiteralProvider;
  };
}
