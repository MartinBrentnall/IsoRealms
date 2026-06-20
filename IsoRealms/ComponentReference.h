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

#include "IsoRealms/Editing/Property/IComponentDefiner.h"
#include "IsoRealms/Editing/Property/ITreeSelectorObject.h"
#include "IsoRealms/IComponentUser.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"

#include "Project/ProjectFile.h"

namespace IsoRealms {
  class Application;

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
        cManager.getResourceManager().release(this, cDefComponent);
      }

      if (id == "") {
        cDefComponent = nullptr;
      } else {
        cDefComponent = cManager.getResourceManager().template get<TYPE>(this, id);
      }
    }

    void save(JSONObject object, const std::string& name) const {
      object.addString(name, cManager.getResourceManager().getComponentID(cDefComponent));
    }

    TreeItemInfo getTreeItemInfo() const override {
      std::string mComponentID = cManager.getResourceManager().getComponentID(cDefComponent);
      std::optional<TreeItemInfo> mFound;
      forEachAvailableTreeItem([&mFound, &mComponentID](const TreeItemInfo& mTreeItemInfo) {
        if (mTreeItemInfo.cID == mComponentID) {
          mFound = mTreeItemInfo;
        }
      });
      return mFound.value_or(TreeItemInfo{mComponentID, mComponentID});
    }

    std::string getTreeItemLabel() const override {
      return cManager.getResourceManager().getComponentID(cDefComponent);
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

    void defineTreeItem(IComponentDefiner& definer) override {
      definer.propertyString("key", [this]() {
        return cManager.getResourceManager().getComponentID(cDefComponent);
      }, [this](const std::string& value) {
        setID(value);
      });
    }

    void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override {
      for (const std::string& mComponentID : cManager.getResourceManager().template getAvailableComponents<TYPE>()) {
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
    void relinquish(TYPE* resource) override {
      if (cDefComponent == resource) {
        cDefComponent = nullptr;
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
    TYPE* cDefComponent;
  };
}