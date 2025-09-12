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

#include "IsoRealms/Project/Project.h"
#include "IsoRealms/PropertyMaker.h"

namespace IsoRealms {
  PropertyNativeBoolean::PropertyNativeBoolean(PropertyMaker& owner, const PropertyData& data, IResourceAccessManager& resourceAccessManager, std::function<bool()> getter, std::function<void(bool)> setter, Project& project, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cInternalSelection(setter, project, getter()),
            cInternalProperty(owner, resourceAccessManager, owner.getResourceData(), data, cInternalSelection, removeFunction) {
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
  
  PropertyNativeBoolean::BooleanSelection::BooleanSelection(std::function<void(bool)> setter, Project& project, bool value) :
            cProject(project),
            cSetter(setter),
            cValue(value) {
  }

  std::string PropertyNativeBoolean::BooleanSelection::getID() const {
    return cValue ? ID_TRUE : ID_FALSE;
  }
  
  bool PropertyNativeBoolean::BooleanSelection::renderAssetIcon() const {
    if (cValue) {
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

  void PropertyNativeBoolean::BooleanSelection::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }
  
  Application& PropertyNativeBoolean::BooleanSelection::getApplication() const {
    return cProject.getApplication();
  }
  
  std::vector<std::string> PropertyNativeBoolean::BooleanSelection::getAvailableProviders() const {
    return std::vector<std::string>{ID_TRUE, ID_FALSE};
  }
  
  bool PropertyNativeBoolean::BooleanSelection::renderProviderIcon(const std::string& id) const {
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
      cValue = true;
    } else if (id == ID_FALSE) {
      cValue = false;
    } else {
      std::cout << "TODO: Throw Unsupported BooleanSelection ID" << std::endl;
    }
    cSetter(cValue);
  }

  const std::string PropertyNativeBoolean::BooleanSelection::ID_TRUE  = "true";
  const std::string PropertyNativeBoolean::BooleanSelection::ID_FALSE = "false";
}
