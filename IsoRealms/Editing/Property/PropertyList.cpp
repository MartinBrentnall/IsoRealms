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

#include "IsoRealms/Project/Project.h"
#include "IsoRealms/PropertyMaker.h"

namespace IsoRealms {
  PropertyList::PropertyList(PropertyMaker& owner, IResourceAccessManager& resourceAccessManager, Project& project, const PropertyData& data, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cInternalSelection(project, options, getter, setter),
            cInternalProperty(owner, resourceAccessManager, owner.getResourceData(), data, cInternalSelection, removeFunction) {
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

  AssetInfo PropertyList::ListSelection::getAssetInfo() const {
    std::string id = cGetter();
    for (const AssetInfo& e : getAvailableProviders()) {
      if (e.cID == id) return e;
    }
    return AssetInfo{id, ""};
  }

  bool PropertyList::ListSelection::renderAssetIcon() const {
    return false;
  }

  bool PropertyList::ListSelection::hasConfiguration() const {
    return false;
  }

  bool PropertyList::ListSelection::isDefaultConfigured() const {
    return true;
  }

  void PropertyList::ListSelection::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }
  
  Application& PropertyList::ListSelection::getApplication() const {
    return cProject.getApplication();
  }
  
  std::vector<AssetInfo> PropertyList::ListSelection::getAvailableProviders() const {
    std::vector<AssetInfo> result;
    for (const std::string& opt : cOptions) {
      result.emplace_back(AssetInfo{opt, opt});
    }
    return result;
  }
  
  bool PropertyList::ListSelection::renderProviderIcon(const std::string& id) const {
    return false;
  }
  
  void PropertyList::ListSelection::setID(const std::string& id) {
    cSetter(id);
  }
}
