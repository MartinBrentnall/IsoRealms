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
#include "IsoRealms/IComponentUser.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"

#include "Project/ProjectFile.h"

namespace IsoRealms {
  class Application;
  class Metadata;

  template <typename TYPE, typename MANAGER> class ComponentReference : public IComponentUser<TYPE>, 
                                                                        public ITreeSelectorObject {
    public:
    ComponentReference(MANAGER& manager) :
              cManager(manager),
              cDefComponent(nullptr) {
    }

    ComponentReference(MANAGER& manager, TYPE* component) :
              cManager(manager),
              cDefComponent(component) {
    }

    virtual ~ComponentReference() {
    }

    TYPE* get() const {
      return cDefComponent;
    }

    TYPE* operator->() const {
      return cDefComponent;
    }

    void setID(const std::string& id) override {
      if (cDefComponent != nullptr) {
        cManager.getAssetManager().release(this, cDefComponent);
      }

      if (id == "") {
        cDefComponent = nullptr;
      } else {
        cDefComponent = cManager.getAssetManager().template get<TYPE>(this, id);
      }
    }

    void save(JSONObject object, const std::string& name) const {
      object.addString(name, cManager.getAssetManager().getComponentID(cDefComponent));
    }

    TreeItemInfo getTreeItemInfo() const override {
      std::string mComponentID = cManager.getAssetManager().getComponentID(cDefComponent);
      std::optional<TreeItemInfo> mFound;
      forEachAvailableTreeItem([&mFound, &mComponentID](const TreeItemInfo& mTreeItemInfo) {
        if (mTreeItemInfo.cID == mComponentID) {
          mFound = mTreeItemInfo;
        }
      });
      return mFound.value_or(TreeItemInfo{mComponentID, mComponentID});
    }

    std::string getTreeItemLabel() const override {
      return cManager.getAssetManager().getComponentID(cDefComponent);
    }

    bool renderTreeItemIcon() const override {
      return false; // TODO: Implement this.
    }

    bool hasConfiguration() const override {
      return false; // TODO: Implement this.
    }

    bool isDefaultConfigured() const override {
      return false; // TODO: Implement this.
    }

    void getTreeItemProperties(IPropertyMaker& owner) override {
      // TODO: Implement this.
    }

    const Metadata& getPropertyMetadata() const override {
      throw std::runtime_error("ComponentReference::getPropertyMetadata: Property metadata is not available for this type.");
    }

    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override {
      for (const std::string& mComponentID : cManager.getAssetManager().template getAvailableComponents<TYPE>()) {
        getTreeItemInfoFunction(TreeItemInfo{mComponentID, mComponentID});
      }
    }

    bool renderTreeItemIcon(const std::string& id) const override {
      return false; // TODO: Implement this.
    }

    Application& getApplication() override {
      return cManager.getProject().getApplication();
    }

    /***********************************\
     * Implements IComponentUser<TYPE> *
    \***********************************/
    void relinquish(TYPE* asset) override {
      if (cDefComponent == asset) {
        cDefComponent = nullptr;
      }
    }

    bool isReadOnly() const override {
      return cManager.isReadOnly();
    }

    void setOwner(ProjectFile* owner) override {
      cManager.setOwner(owner);
    }

    void loadFromProperty(JSONObject object, const std::string& key, const Options& hint) override {
      if (hint.getOption(Options::PROPERTY_IMMEDIATE) == "true" || cManager.getProject().areComponentsLoaded()) {
        setID(object.getString(key));
      } else {
        cManager.getProject().init([this, object, key]() {
          setID(object.getString(key));
        });
      }
    }

    void loadFromProperty(JSONObject object, const Options& hint) override {
      throw std::runtime_error("ComponentReference::loadFromProperty: Not implemented.");
    }

    void saveToProperty(JSONObject object, const std::string& key, const Options& hint) const override {
      save(object, key);
    }

    private:

    // External interfaces.
    MANAGER& cManager;

    // Definition data.
    TYPE* cDefComponent;
  };
}