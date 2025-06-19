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
#include "Action.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IResourceData.h"

namespace IsoRealms {
  Action::Action(IResourceData& owner) :
            cProject(owner.getProject()),
            cManager(owner),
            cAction(cProject.createLiteralAction(this, owner)) {
  }

  void Action::init(JSONObject object, const std::string& tag, IBindingRegistry* localArgs) {
    if (object.hasMember(tag)) {
      cProject.init([this, object, tag, localArgs](IAssets& assets) {
        set(object, tag, localArgs);
      });
    }
  }

  void Action::set(JSONObject object, const std::string& tag, IBindingRegistry* localArgs) {
    cProject.release(this, cAction);
    cAction = cProject.getAction(this, object, cManager, tag, localArgs);
  }

  void Action::setID(const std::string& id) {
    cProject.release(this, cAction);
    cAction = cProject.getAction(this, id, cManager);
  }

  void Action::execute() {
    cAction->execute();
  }

  std::string Action::getID() const {
    return cAction->getID(cProject);
  }

  std::vector<std::string> Action::getAvailableProviders() const {
    return cProject.getAllActionTypes();
  }  

  bool Action::renderProviderIcon(const std::string& id) const {
    return cProject.renderActionIcon(id);
  }

  bool Action::hasConfiguration() const {
    return cAction->hasConfiguration();
  }
  
  bool Action::isDefaultConfigured() const {
    return cAction->isDefaultConfiguration();
  }

  bool Action::renderAssetIcon() const {
    return cAction->renderAssetIcon();
  }

  void Action::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    cAction->save(mAssetObject, cProject);
  }

  std::vector<std::unique_ptr<IProperty>> Action::getAssetProperties() {
    return cAction->getAssetProperties();
  }

  void Action::relinquish(ActionExecutor* asset) {
    if (cAction == asset) {
      cAction = cProject.createLiteralAction(this, cManager);
    }
  }

  Action::~Action() {
    if (cAction != nullptr) {
      cProject.release(this, cAction);
    }
  }
}
