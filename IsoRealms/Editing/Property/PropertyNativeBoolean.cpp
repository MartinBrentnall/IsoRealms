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
#include "PropertyNativeBoolean.h"

#include <stdexcept>

#include "IsoRealms/Metadata.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/PropertyMaker.h"

namespace IsoRealms {
  PropertyNativeBoolean::PropertyNativeBoolean(IPropertyMaker& owner, const PropertyData& data, IComponentAccessManager& resourceAccessManager, std::function<bool()> getter, std::function<void(bool)> setter, Project& project, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cInternalSelection(setter, getter, project),
            cInternalProperty(owner, resourceAccessManager, owner.getComponentData(), data, cInternalSelection, removeFunction) {
  }

  void PropertyNativeBoolean::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    cInternalProperty.renderValue(style, y, x, aspectRatio);
  }

  float PropertyNativeBoolean::getValueWidth(IUIStyle& style) const {
    return cInternalProperty.getValueWidth(style);
  }

  void PropertyNativeBoolean::confirm(IPropertyManager& manager, float y) {
    cInternalProperty.confirm(manager, y);
  }

  bool PropertyNativeBoolean::hasConfiguration() const {
    return cInternalProperty.hasConfiguration();
  }

  void PropertyNativeBoolean::configure(IPropertyManager& manager) {
    cInternalProperty.configure(manager);
  }
  
  PropertyNativeBoolean::BooleanSelection::BooleanSelection(std::function<void(bool)> setter, std::function<bool()> getter, Project& project) :
            cProject(project),
            cSetter(setter),
            cGetter(getter) {
  }

  TreeItemInfo PropertyNativeBoolean::BooleanSelection::getTreeItemInfo() const {
    return cGetter() ? TreeItemInfo{ID_TRUE, "True"} : TreeItemInfo{ID_FALSE, "False"};
  }
  
  std::string PropertyNativeBoolean::BooleanSelection::getTreeItemLabel() const {
    return cGetter() ? "True" : "False";
  } 
  
  bool PropertyNativeBoolean::BooleanSelection::renderAssetIcon() const {
    if (cGetter()) {
      Utils::renderIconTick();
    } else {
      Utils::renderIconNone();
    }
    return true;
  }
  
  bool PropertyNativeBoolean::BooleanSelection::hasConfiguration() const {
    return false;
  }

  bool PropertyNativeBoolean::BooleanSelection::isDefaultConfigured() const {
    return true;
  }

  void PropertyNativeBoolean::BooleanSelection::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  const Metadata& PropertyNativeBoolean::BooleanSelection::getPropertyMetadata() const {
    throw std::runtime_error("PropertyNativeBoolean::BooleanSelection::getPropertyMetadata: Property metadata is not available for this type.");
  }
  
  Application& PropertyNativeBoolean::BooleanSelection::getApplication() {
    return cProject.getApplication();
  }
  
  void PropertyNativeBoolean::BooleanSelection::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    getTreeItemInfoFunction(TreeItemInfo{ID_TRUE, "True"});
    getTreeItemInfoFunction(TreeItemInfo{ID_FALSE, "False"}); // TODO: Localisation.
  }
  
  bool PropertyNativeBoolean::BooleanSelection::renderTreeItemIcon(const std::string& id) const {
    if (id == ID_TRUE) {
      Utils::renderIconTick();
    } else if (id == ID_FALSE) {
      Utils::renderIconNone();
    } else {
      std::cout << "TODO: Throw Unsupported BooleanSelection ID" << std::endl;
    }
    return true;
  }
  
  void PropertyNativeBoolean::BooleanSelection::setID(const std::string& id) {
    if (id == ID_TRUE) {
      cSetter(true);
    } else if (id == ID_FALSE) {
      cSetter(false);
    } else {
      std::cout << "TODO: Throw Unsupported BooleanSelection ID" << std::endl;
    }
  }
}
