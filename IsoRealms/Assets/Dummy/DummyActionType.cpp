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
#include "DummyActionType.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyActionType::DummyActionType(IResourceData& owner) :
    DUMMY() {
  }
  
  IAction* DummyActionType::createAction(JSONObject object, IResourceData& owner, IBindingRegistry* localObjects) {
    return &DUMMY;
  }

  IAction* DummyActionType::createAction(IResourceData& owner, IBindingRegistry* localArgs) {
    return &DUMMY;
  }
  
  void DummyActionType::destroyAction(IAction* action, IAssets& assets) {
    // Nothing to do.
  }

  bool DummyActionType::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }
  
  void DummyActionType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> DummyActionType::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool DummyActionType::isDefaultConfiguration() const {
    return true;
  }

  DummyActionType::Action::Action() {
    // Nothing to do.
  }
  
  void DummyActionType::Action::execute() {
    // Do nothing.
  }
  
  void DummyActionType::Action::save(JSONObject object, IAssetIdentifier& identifier) const {
    // Nothing to do.
  }

  bool DummyActionType::Action::hasConfiguration() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> DummyActionType::Action::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    // TODO: Implement this.
    return mProperties;
  }

  bool DummyActionType::Action::isDefaultConfiguration() const {
    return true;
  }
}
