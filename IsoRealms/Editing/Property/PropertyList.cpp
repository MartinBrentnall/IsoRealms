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
#include "PropertyList.h"

namespace IsoRealms {
  PropertyList::PropertyList(IProject& project, const std::string& name, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction) :
            Property(name, removeFunction),
            cInternalSelection(project, options, getter, setter),
            cInternalProperty(name, cInternalSelection, removeFunction) {
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

  PropertyList::ListSelection::ListSelection(IProject& project, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter) :
            cProject(project),
            cOptions(options),
            cGetter(getter),
            cSetter(setter) {
  }

  std::string PropertyList::ListSelection::getID() const {
    return cGetter();
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

  std::vector<std::unique_ptr<IProperty>> PropertyList::ListSelection::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }
  
  IApplication& PropertyList::ListSelection::getApplication() const {
    return cProject.getApplication();
  }
  
  std::vector<std::string> PropertyList::ListSelection::getAvailableProviders() const {
    return cOptions;
  }
  
  bool PropertyList::ListSelection::renderProviderIcon(const std::string& id) const {
    return false;
  }
  
  void PropertyList::ListSelection::setID(const std::string& id) {
    cSetter(id);
  }
}
