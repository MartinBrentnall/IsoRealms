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
#include "Property.h"

namespace IsoRealms {
  Property::Property(const PropertyData& data, IComponentAccessManager& resourceAccessManager, std::function<void()> removeFunction) :
            cData(data),
            cConfirmationManager(resourceAccessManager),
            cRemoveFunction(removeFunction) {
  }

  std::string Property::getPropertyName() const {
    return cData.getName();
  }

  void Property::confirmAccess(std::function<void()> confirm, std::function<void()> cancel) {
    if (cConfirmationManager.isComponentReadOnly()) {
      cConfirmationManager.confirm("TODO: The component you're modifying is read-only.  In order to modify this resource, it will need to be promoted to the main project file.", [this, confirm]() {
        cConfirmationManager.promoteComponentToProject();
        confirm();
      }, [this, cancel]() {
        cancel();
      });
    } else {
      confirm();
    }
  }

  bool Property::isRemovable() const {
    return cRemoveFunction != nullptr;
  }
  
  void Property::remove() {
    cRemoveFunction();
  }

  std::string Property::getTooltip() const {
    return cData.getTooltip();
  }
}
