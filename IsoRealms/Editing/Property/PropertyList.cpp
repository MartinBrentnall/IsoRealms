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
#include "PropertyList.h"

#include <optional>
#include <stdexcept>

#include "IsoRealms/Metadata.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/PropertyMaker.h"

namespace IsoRealms {
  PropertyList::PropertyList(IPropertyMaker& owner, IComponentAccessManager& resourceAccessManager, Project& project, const PropertyData& data, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cInternalSelection(project, options, getter, setter),
            cInternalProperty(owner, resourceAccessManager, owner.getComponentData(), data, cInternalSelection, removeFunction) {
  }
  
  void PropertyList::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    cInternalProperty.renderValue(style, y, x, aspectRatio);
  }

  float PropertyList::getValueWidth(IUIStyle& style) const {
    return cInternalProperty.getValueWidth(style);
  }

  void PropertyList::confirm(IPropertyManager& manager, float y) {
    cInternalProperty.confirm(manager, y);
  }

  bool PropertyList::hasConfiguration() const {
    return cInternalProperty.hasConfiguration();
  }
  
  void PropertyList::configure(IPropertyManager& manager) {
    cInternalProperty.configure(manager);
  }

  PropertyList::ListSelection::ListSelection(Project& project, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter) :
            cProject(project),
            cOptions(options),
            cGetter(getter),
            cSetter(setter) {
  }

  TreeItemInfo PropertyList::ListSelection::getTreeItemInfo() const {
    std::string mSelectedID = cGetter();
    std::optional<TreeItemInfo> mFound;
    forEachAvailableTreeItem([&mFound, &mSelectedID](const TreeItemInfo& mTreeItemInfo) {
      if (mTreeItemInfo.cID == mSelectedID) {
        mFound = mTreeItemInfo;
      }
    });
    return mFound.value_or(TreeItemInfo{mSelectedID, mSelectedID});
  }

  std::string PropertyList::ListSelection::getTreeItemLabel() const {
    return cGetter();
  }

  bool PropertyList::ListSelection::renderTreeItemIcon() const {
    return false;
  }

  bool PropertyList::ListSelection::hasConfiguration() const {
    return false;
  }

  bool PropertyList::ListSelection::isDefaultConfigured() const {
    return true;
  }

  void PropertyList::ListSelection::getTreeItemProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  const Metadata& PropertyList::ListSelection::getPropertyMetadata() const {
    throw std::runtime_error("PropertyList::ListSelection::getPropertyMetadata: Property metadata is not available for this type.");
  }
  
  Application& PropertyList::ListSelection::getApplication() {
    return cProject.getApplication();
  }
  
  void PropertyList::ListSelection::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    for (const std::string& mOption : cOptions) {
      getTreeItemInfoFunction(TreeItemInfo{mOption, mOption});
    }
  }
  
  bool PropertyList::ListSelection::renderTreeItemIcon(const std::string& id) const {
    return false;
  }
  
  void PropertyList::ListSelection::setID(const std::string& id) {
    cSetter(id);
  }
}
