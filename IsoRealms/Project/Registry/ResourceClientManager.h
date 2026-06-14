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

#include <stdexcept>

#include "IsoRealms/Resources/Providers/ResourceSingleton.h"
#include "IsoRealms/Resources/Type/IStateNotifier.h"
#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Persistence/JSONObject.h"

#include "ResourceRegistry.h"

namespace IsoRealms {
  class Metadata;

  template <typename DERIVED, typename OWNER, typename TYPE> class ResourceClientManager {
    public:
    inline static const std::string JSON_KEY = "key";

    ResourceRegistry<OWNER, TYPE> cRegistry;

    ResourceClientManager() :
              ResourceClientManager(static_cast<IResourceProvider<OWNER, TYPE>*>(nullptr)) {
    }

    ResourceClientManager(TYPE* defaultResource, const std::string& defaultProviderID = "None", const std::string& defaultProviderName = "None") {
      cDefaultProvider = cResourceSingletons.emplace(defaultResource, std::make_unique<ResourceSingleton<OWNER, TYPE>>(defaultProviderID, defaultResource)).first->second.get();
      add(cDefaultProvider, defaultProviderID, defaultProviderName, false);
    }

    ResourceClientManager(IResourceProvider<OWNER, TYPE>* defaultProvider, const std::string& defaultProviderID = "None", const std::string& defaultProviderName = "None") :
              cDefaultProvider(defaultProvider) {
      if (cDefaultProvider != nullptr) {
        add(cDefaultProvider, defaultProviderID, defaultProviderName);
      }
    }

    IStateNotifier* add(TYPE* resource, const std::string& id, const std::string& path = "TODO", bool stateChanges = false) {
      typename std::map<const TYPE*, std::unique_ptr<ResourceSingleton<OWNER, TYPE>>>::iterator mIterator = cResourceSingletons.find(resource);
      if (mIterator == cResourceSingletons.end()) {
        return add(cResourceSingletons.emplace(resource, std::make_unique<ResourceSingleton<OWNER, TYPE>>(id, resource)).first->second.get(), id, path, stateChanges);
      }
      return add(mIterator->second.get(), id, path, stateChanges);
    }
    
    IStateNotifier* add(IResourceProvider<OWNER, TYPE>* provider, const std::string& id, const std::string& path, bool stateChanges = false) {
      cRegistry.add(provider, id, path);
      if (stateChanges) {
        typename std::map<const IResourceProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(provider);
        if (mNotifier != cStateNotifiers.end()) {
          return mNotifier->second.get();
        }
        return cStateNotifiers.emplace(provider, std::make_unique<StateNotifier>()).first->second.get();
      }
      return nullptr;
    }
    
    void remove(TYPE* resource) {
      typename std::map<const TYPE*, std::unique_ptr<ResourceSingleton<OWNER, TYPE>>>::iterator mIterator = cResourceSingletons.find(resource);
      if (mIterator == cResourceSingletons.end()) {
        std::cout << "WARNING: ResourceClientManager::remove: Specified resource has no singleton in this registry." << std::endl;
      } else {
        remove(mIterator->second.get());
        cResourceSingletons.erase(resource);
//        cRegistry.checkClean("DEBUG");
      }
    }

    void remove(IResourceProvider<OWNER, TYPE>* provider) {
      typename std::map<IResourceProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IResourceUser<TYPE>*>>>::iterator mIterator = cClients.find(provider);
      if (mIterator != cClients.end()) {
        for (std::pair<TYPE*, std::vector<IResourceUser<TYPE>*>> mPair : mIterator->second) {
          for (IResourceUser<TYPE>* mClient : mPair.second) {
            mClient->relinquish(mPair.first);
          }
        }
        cClients.erase(provider);
      }
      cStateNotifiers.erase(provider);
      cRegistry.remove(provider);
    }
    
    bool hasReadOnlyReferences(TYPE* resource) const {
      typename std::map<const TYPE*, std::unique_ptr<ResourceSingleton<OWNER, TYPE>>>::const_iterator mIterator = cResourceSingletons.find(resource);
      if (mIterator == cResourceSingletons.end()) {
        std::cout << "WARNING: ResourceClientManager::hasReadOnlyReferences: Specified resource has no singleton in this registry." << std::endl;
      } else {
        return hasReadOnlyReferences(mIterator->second.get());
      }
      return false;
    }
    
    bool hasReadOnlyReferences(IResourceProvider<OWNER, TYPE>* provider) const {
      typename std::map<IResourceProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IResourceUser<TYPE>*>>>::const_iterator mIterator = cClients.find(provider);
      if (mIterator != cClients.end()) {
        for (std::pair<TYPE*, std::vector<IResourceUser<TYPE>*>> mPair : mIterator->second) {
          for (IResourceUser<TYPE>* mClient : mPair.second) {
            if (mClient->isReadOnly()) {
              return true;
            }
          }
        }
      }
      return false;
    }
    
    void overrideReadOnlyReferences(TYPE* resource, ProjectFile* owner) {
      typename std::map<const TYPE*, std::unique_ptr<ResourceSingleton<OWNER, TYPE>>>::iterator mIterator = cResourceSingletons.find(resource);
      if (mIterator == cResourceSingletons.end()) {
        std::cout << "WARNING: ResourceClientManager::overrideReadOnlyReferences: Specified resource has no singleton in this registry." << std::endl;
      } else {
        return overrideReadOnlyReferences(mIterator->second.get(), owner);
      }
    }
    
    void overrideReadOnlyReferences(IResourceProvider<OWNER, TYPE>* provider, ProjectFile* owner) {
      typename std::map<IResourceProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IResourceUser<TYPE>*>>>::iterator mIterator = cClients.find(provider);
      if (mIterator != cClients.end()) {
        for (std::pair<TYPE*, std::vector<IResourceUser<TYPE>*>> mPair : mIterator->second) {
          for (IResourceUser<TYPE>* mClient : mPair.second) {
            if (mClient->isReadOnly()) {
              mClient->setOwner(owner);
            }
          }
        }
      }
    }
    
    TYPE* getDefault(IResourceUser<TYPE>* client, OWNER& owner) {
      TYPE* mResource = cDefaultProvider != nullptr ? cDefaultProvider->getResource(owner) : nullptr;
      if (mResource != nullptr) {
        cClients[cDefaultProvider][mResource].emplace_back(client);
      }
      return mResource;
    }

    TYPE* get(IResourceUser<TYPE>* client, OWNER& owner, JSONObject object, IStateListener* listener, bool required) {
      if (client == nullptr) {
        throw std::invalid_argument("Client cannot be null");
      }

      TYPE* mResource = nullptr;
      IResourceProvider<OWNER, TYPE>* mProvider = cRegistry.getProvider(object.getString(JSON_KEY), required);
      if (mProvider != nullptr) {
        mResource = mProvider->getResource(owner, object);
      }
      registerClient(client, mProvider, mResource, listener);
      return mResource;
    }

    void registerClient(IResourceUser<TYPE>* client, IResourceProvider<OWNER, TYPE>* provider, TYPE* resource, IStateListener* listener = nullptr) {
      if (resource != nullptr) {
        cClients[provider][resource].emplace_back(client);

        if (listener != nullptr) {
          typename std::map<const IResourceProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(provider);
          if (mNotifier != cStateNotifiers.end()) {
            mNotifier->second->addListener(listener);
          }
        }
      }
    }

    TYPE* get(IResourceUser<TYPE>* client, OWNER& owner, const std::string& id, IStateListener* listener) {
      if (client == nullptr) {
        throw std::invalid_argument("Client cannot be null");
      }

      IResourceProvider<OWNER, TYPE>* mProvider = cRegistry.getProvider(id, true);
      TYPE* mResource = mProvider->getResource(owner);
      registerClient(client, mProvider, mResource, listener);
      return mResource;
    }

    void addStateChangeListener(const TYPE* resource, IStateListener* listener) {
      typename std::map<const IResourceProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>>::iterator mNotifier = cStateNotifiers.find(getProvider(resource));
      if (mNotifier != cStateNotifiers.end()) {
        mNotifier->second->addListener(listener);
      }
    }
    
    virtual void forEachEntry(const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const {
      cRegistry.forEachEntry(getTreeItemInfoFunction);
    }
    
    void release(IResourceUser<TYPE>* client, TYPE* resource) {
      if (client == nullptr) {
        std::cout << "WARNING: Client from which to release cannot is nullptr" << std::endl;
        return;
      }
      if (resource == nullptr) {
        std::cout << "WARNING: Resource to release is nullptr" << std::endl;
        return;
      }
      IResourceProvider<OWNER, TYPE>* mProvider = getProvider(resource);
      typename std::map<IResourceProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IResourceUser<TYPE>*>>>::iterator mIterator = cClients.find(mProvider);
      if (mIterator == cClients.end()) {
        std::cout << "WARNING: Specified resource isn't registered with specified client" << std::endl;
        return;
      }
      
      typename std::map<TYPE*, std::vector<IResourceUser<TYPE>*>>::iterator mResourceClientsIterator = mIterator->second.find(resource);
      unsigned int mClientIndex = std::find(mResourceClientsIterator->second.begin(), mResourceClientsIterator->second.end(), client) - mResourceClientsIterator->second.begin();
      if (mClientIndex == mResourceClientsIterator->second.size()) {
        std::cout << "WARNING: Specified client isn't registered as a user of the specified resource" << std::endl;
        return;
      }

      mResourceClientsIterator->second.erase(mResourceClientsIterator->second.begin() + mClientIndex);
      if (mResourceClientsIterator->second.empty()) {
        mIterator->second.erase(resource);
        if (mIterator->second.empty()) {
          cClients.erase(mProvider);
        }
      }

      mProvider->releaseResource(resource);
      // TODO: Remove state listener (if there is one) from state notifier
    }
    
    TreeItemInfo getTreeItemInfo(const TYPE* resource) const {
      return cRegistry.getTreeItemInfo(getProvider(resource));
    }

    void save(JSONObject object, const TYPE* resource) const {
      const IResourceProvider<OWNER, TYPE>* mProvider = getProvider(resource);
      std::string mID = cRegistry.getTreeItemInfo(mProvider).cID;
      object.addString(JSON_KEY, mID);
      resource->saveResource(object);
    }

    void addResourceListener(IResourceListener<OWNER, TYPE>* listener) {
      cRegistry.addResourceListener(listener);
    }
    
    virtual bool renderIcon(const std::string& id) const {
      return cRegistry.renderIcon(id);
    }
    
    bool hasConfiguration(const std::string& id) const {
      return cRegistry.getProvider(id, true)->hasConfiguration();
    }

    const Metadata& getPropertyMetadata(const TYPE* resource) const {
      IResourceProvider<OWNER, TYPE>* mProvider = getProvider(resource);
      if (mProvider == nullptr) {
        throw std::runtime_error("ResourceClientManager::getPropertyMetadata: No provider found for specified resource.");
      }
      return mProvider->getMetadata();
    }
    
    ResourceRegistry<OWNER, TYPE>* getRegistry() {
      return &cRegistry;
    }

    void checkClean(const std::string& name) {
      cRegistry.checkClean(name);
      if (!cClients.empty()) {
        std::cout << "WARNING: ResourceClientManager::checkClean: " << cClients.size() << " " << name << " providers are still in use." << std::endl;
      }
      if (!cStateNotifiers.empty()) {
        std::cout << "WARNING: ResourceClientManager::checkClean: " << cStateNotifiers.size() << " " << name << " have not been removed." << std::endl;
      }
    }
    
    ~ResourceClientManager() {
      cDefaultProvider = nullptr;
      for (std::pair<const TYPE* const, std::unique_ptr<ResourceSingleton<OWNER, TYPE>>>& mPair : cResourceSingletons) {
        remove(mPair.second.get());
      }
      for (std::pair<const IResourceProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IResourceUser<TYPE>*>>> mPairA : cClients) {
        for (std::pair<TYPE*, std::vector<IResourceUser<TYPE>*>> mPairB : mPairA.second) {
          for (IResourceUser<TYPE>* mClient : mPairB.second) {
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

    IResourceProvider<OWNER, TYPE>* getProvider(const TYPE* resource) const {
      for (std::pair<IResourceProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IResourceUser<TYPE>*>>> mPairA : cClients) {
        for (std::pair<TYPE*, std::vector<IResourceUser<TYPE>*>> mPairB : mPairA.second) {
          if (mPairB.first == resource) {
            return mPairA.first;
          }
        }
      }

      // Special case where resource is a singleton that has no clients yet.
      typename std::map<const TYPE*, std::unique_ptr<ResourceSingleton<OWNER, TYPE>>>::const_iterator mIterator = cResourceSingletons.find(resource);
      if (mIterator != cResourceSingletons.end()) {
        return mIterator->second.get();
      }
      return nullptr;
    }

//     class ProviderData {
//       private:
//       std::optional<StateNotifier> cStateNotifier;
//       std::map<TYPE*, IResourceUser<TYPE*>> cClients;
//     };
    
    std::map<const IResourceProvider<OWNER, TYPE>*, std::unique_ptr<StateNotifier>> cStateNotifiers;
    std::map<IResourceProvider<OWNER, TYPE>*, std::map<TYPE*, std::vector<IResourceUser<TYPE>*>>> cClients;
    std::map<const TYPE*, std::unique_ptr<ResourceSingleton<OWNER, TYPE>>> cResourceSingletons;
    IResourceProvider<OWNER, TYPE>* cDefaultProvider;
  };
}
