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

#include "IsoRealms/IResourceUser.h"
#include "IsoRealms/Project/Registry/AssetInfo.h"

#include "Project/ProjectFile.h"

namespace IsoRealms {
  template <typename TYPE, typename MANAGER> class ResourceReference : public IResourceUser<TYPE> {
    public:
    ResourceReference(MANAGER& manager) :
              cManager(manager),
              cDefResource(nullptr) {
    }

    ResourceReference(MANAGER& manager, TYPE* resource) :
              cManager(manager),
              cDefResource(resource) {
    }

    virtual ~ResourceReference() {
    }

    TYPE* get() const {
      return cDefResource;
    }

    TYPE* operator->() const {
      return cDefResource;
    }

    void setID(const std::string& id) {
      if (cDefResource != nullptr) {
        cManager.getAssetManager().release(this, cDefResource);
      }

      if (id == "") {
        cDefResource = nullptr;
      } else {
        cDefResource = cManager.getAssetManager().template get<TYPE>(this, id);
      }
    }

    void save(JSONObject object, const std::string& name) const {
      object.addString(name, cManager.getAssetManager().getResourceID(cDefResource));
    }

    AssetInfo getAssetInfo() const {
      std::string id = cManager.getAssetManager().getResourceID(cDefResource);
      for (const AssetInfo& e : getAvailableProviders()) {
        if (e.cID == id) return e;
      }
      return AssetInfo{id, ""};
    }

    bool renderAssetIcon() const {
      return false; // TODO: Implement this.
    }

    bool hasConfiguration() const {
      return false; // TODO: Implement this.
    }

    bool isDefaultConfigured() const {
      return false; // TODO: Implement this.
    }

    void getAssetProperties(PropertyMaker& owner) {
      // TODO: Implement this.
    }

    std::vector<AssetInfo> getAvailableProviders() const {
      std::vector<AssetInfo> mResult;
      for (const std::string& id : cManager.getAssetManager().template getAvailableResources<TYPE>()) {
        mResult.emplace_back(AssetInfo{id, id});
      }
      return mResult;
    }

    bool renderProviderIcon(const std::string& id) const {
      return false; // TODO: Implement this.
    }

    Application& getApplication() const {
      return cManager.getProject().getApplication();
    }

    /**********************************\
     * Implements IResourceUser<TYPE> *
    \**********************************/
    void relinquish(TYPE* asset) override {
      if (cDefResource == asset) {
        cDefResource = nullptr;
      }
    }

    bool isReadOnly() const override {
      return cManager.isReadOnly();
    }

    void setOwner(ProjectFile* owner) override {
      cManager.setOwner(owner);
    }

    private:

    // External interfaces.
    MANAGER& cManager;

    // Definition data.
    TYPE* cDefResource;
  };
}