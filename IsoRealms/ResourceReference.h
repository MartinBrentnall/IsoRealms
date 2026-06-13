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
#include <optional>
#include <stdexcept>

#include "IsoRealms/Editing/Property/ITreeSelectorObject.h"
#include "IsoRealms/IResourceUser.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"

#include "Project/ProjectFile.h"

namespace IsoRealms {
  class Application;
  class Metadata;

  template <typename TYPE, typename MANAGER> class ResourceReference : public IResourceUser<TYPE>, 
                                                                       public ITreeSelectorObject {
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

    void setID(const std::string& id) override {
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

    TreeItemInfo getTreeItemInfo() const override {
      std::string mResourceID = cManager.getAssetManager().getResourceID(cDefResource);
      std::optional<TreeItemInfo> mFound;
      forEachAvailableTreeItem([&mFound, &mResourceID](const TreeItemInfo& mTreeItemInfo) {
        if (mTreeItemInfo.cID == mResourceID) {
          mFound = mTreeItemInfo;
        }
      });
      return mFound.value_or(TreeItemInfo{mResourceID, mResourceID});
    }

    std::string getTreeItemLabel() const override {
      return cManager.getAssetManager().getResourceID(cDefResource);
    }

    bool renderAssetIcon() const override {
      return false; // TODO: Implement this.
    }

    bool hasConfiguration() const override {
      return false; // TODO: Implement this.
    }

    bool isDefaultConfigured() const override {
      return false; // TODO: Implement this.
    }

    void getAssetProperties(IPropertyMaker& owner) override {
      // TODO: Implement this.
    }

    const Metadata& getPropertyMetadata() const override {
      throw std::runtime_error("ResourceReference::getPropertyMetadata: Property metadata is not available for this type.");
    }

    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override {
      for (const std::string& mResourceID : cManager.getAssetManager().template getAvailableResources<TYPE>()) {
        getTreeItemInfoFunction(TreeItemInfo{mResourceID, mResourceID});
      }
    }

    bool renderTreeItemIcon(const std::string& id) const override {
      return false; // TODO: Implement this.
    }

    Application& getApplication() override {
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

    void loadFromProperty(JSONObject object, const std::string& key, const Options& hint) override {
      if (hint.getOption(Options::PROPERTY_IMMEDIATE) == "true" || cManager.getProject().areResourcesLoaded()) {
        setID(object.getString(key));
      } else {
        cManager.getProject().init([this, object, key]() {
          setID(object.getString(key));
        });
      }
    }

    void loadFromProperty(JSONObject object, const Options& hint) override {
      throw std::runtime_error("ResourceReference::loadFromProperty: Not implemented.");
    }

    private:

    // External interfaces.
    MANAGER& cManager;

    // Definition data.
    TYPE* cDefResource;
  };
}