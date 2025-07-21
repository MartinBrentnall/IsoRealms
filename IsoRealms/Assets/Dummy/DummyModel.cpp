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
#include "DummyModel.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyModel::ModelInstance DummyModel::DUMMY;
  
  DummyModel::DummyModel(IResourceData& owner) {
    // Nothing to do.
  }

  IModelInstance* DummyModel::createModel() {
    return &DUMMY;
  }
  
  bool DummyModel::renderPreview() const {
    return false;
  }

  bool DummyModel::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void DummyModel::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> DummyModel::getAssetProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool DummyModel::isDefaultConfiguration() const {
    return true;
  }

  void DummyModel::ModelInstance::update(unsigned int milliseconds) {
    // Nothing to do
  }
  
  void DummyModel::ModelInstance::render() const {
    // TODO: Dummy implementation
  }
}
