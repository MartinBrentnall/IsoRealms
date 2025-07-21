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
#include "PropertyMaker.h"

#include "Assets/Registry/IAssetProvider.h"
#include "Assets/Type/IStateNotifier.h"
#include "Editing.h"
#include "IResourceData.h"

namespace IsoRealms {
  PropertyMaker::PropertyMaker(IResourceData& parent, IConfirmationManager& confirmationManager) :
            cParent(parent),
            cConfirmationManager(confirmationManager) {
  }

  Project& PropertyMaker::getProject() {
    return cParent.getProject();
  }

  IActionClient& PropertyMaker::getDummyActionClient() {
    return cParent.getDummyActionClient();
  }

  const PropertyData& PropertyMaker::getPropertyData(const std::string& key) const {
    return cParent.getPropertyData(key);
  }

  std::string PropertyMaker::getPropertyName(const std::string& key) const {
    return cParent.getPropertyName(key);
  }

  std::string PropertyMaker::getPropertyDescription(const std::string& key) const {
    return cParent.getPropertyDescription(key);
  }

  std::unique_ptr<IProperty> PropertyMaker::createPropertyNativeFloat(const std::string& metadataKey, std::function<float()> getter, std::function<bool(float)> setter, std::function<void()> removeFunction) {
    return make_unique<PropertyNativeFloat>(cParent.getPropertyData(metadataKey), &cConfirmationManager, getter, setter, removeFunction);
  }
}

