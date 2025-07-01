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
#include "ActionExecutor.h"

#include "Project.h"

namespace IsoRealms {
  ActionExecutor::ActionExecutor(Project& project, IAssetUser<ActionExecutor>* user) :
            cProject(project),
            cActionType(cProject, [this]() {
              cAction = cActionType->createAction(cProject, nullptr);
            }),
            cAction(cActionType->createAction(cProject, nullptr)),
            cUser(user) {
            // TODO: cUser is never used!
  }

  ActionExecutor::ActionExecutor(Project& project, IAssetUser<ActionExecutor>* user, const std::string& id) :
            cProject(project),
            cActionType(cProject, [this]() {
              cAction = cActionType->createAction(cProject, nullptr);
            }, id),
            cAction(cActionType->createAction(cProject, nullptr)),
            cUser(user) {
            // TODO: cUser is never used!
  }

  ActionExecutor::ActionExecutor(Project& project, IAssetUser<ActionExecutor>* user, JSONObject object, IBindingRegistry* localArgs) :
            cProject(project),
            cActionType(cProject, [this]() {
              cAction = cActionType->createAction(cProject, nullptr);
            }, object),
            cAction(cActionType->createAction(object, cProject, localArgs)),
            cUser(user) {
            // TODO: cUser is never used!
  }

  ActionExecutor::~ActionExecutor() {
    cActionType->destroyAction(cAction, cProject);
  }

  void ActionExecutor::execute() {
    if (cProject.isProcessingInput()) {
      cProject.postponeAction(cAction);
    } else {
      cAction->execute();
    }
  }

  void ActionExecutor::postpone() {
    cProject.postponeAction(cAction);
  }

  std::string ActionExecutor::getID() const {
    return cProject.getID(*cActionType);
  }

  void ActionExecutor::save(JSONObject object) const {
    cProject.save(object, *cActionType);
    cAction->save(object);
  }

  bool ActionExecutor::renderAssetIcon() const {
    return cActionType->renderAssetIcon();
  }

  bool ActionExecutor::hasConfiguration() const {
    return cAction->hasConfiguration();
  }

  std::vector<std::unique_ptr<IProperty>> ActionExecutor::getAssetProperties() {
    return cAction->getAssetProperties();
  }

  bool ActionExecutor::isDefaultConfiguration() const {
    return cAction->isDefaultConfiguration();
  }
}
