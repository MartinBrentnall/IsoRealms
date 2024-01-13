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
#include "PropertyAction.h"

namespace IsoRealms {
  PropertyAction::PropertyAction(const std::string& name, IAction* selected, IAssetUser<IAction>* client, IAssetBrowser* browser, IBindingRegistry* clientObjects, std::function<IAction*(IAction* action)> confirmationCallback) :
            PropertyAsset(name, selected != nullptr ? browser->getID(selected->getActionType()) : "#", browser->getAllActionTypes(), std::vector<IAssetMenuItem*>({
              &cDummyAction
            }), [this](const std::string& id) {
              confirmSelection(id);
              return true;
            }),
            cClient(client),
            cBrowser(browser),
            cClientObjects(clientObjects),
            cConfirmationCallback(confirmationCallback),
            cDummyAction(this) {
  }

  bool PropertyAction::renderIcon(const std::string& id) const {
    return cBrowser->renderActionIcon(id);
  }
  
  PropertyAction::DummyAction::DummyAction(PropertyAction* parent) :
            cParent(parent) {
  }
  
  void PropertyAction::DummyAction::render(float yOffset) const {
    glPushMatrix();
    cParent->transformForIcon(yOffset);
    Utils::renderIconNone();
    glPopMatrix();
    cParent->print("None", yOffset, cParent->getIconWidth() + cParent->getIconTextSpacing());
  }
  
  bool PropertyAction::DummyAction::executeAction() {
    cParent->confirmSelection("#");
    return true;
  }
  
  float PropertyAction::DummyAction::getWidth() const {
    return cParent->getTextWidth("None") + cParent->getIconWidth() + cParent->getIconWidth();
  }
  
  std::vector<std::unique_ptr<PropertyAsset::ISelection>> PropertyAction::DummyAction::getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const {
    std::vector<std::unique_ptr<PropertyAsset::ISelection>> mSelection;
    if (id[0] == '#') {
      mSelection.emplace_back(std::make_unique<AssetMenu::Selection>(menu, item, 0.0f));
    }
    return mSelection;
  }
  
  std::string PropertyAction::confirmSelection(const std::string& id) {
//    IActionType* mActionType = cBrowser->getActionType(cClient, id);
//    IAction* mAction = mActionType->createAction(cBrowser->getProject(), cBrowser->getGlobalObjects(), cClientObjects);
//    if (mAction->hasConfiguration()) {
      // FIXME:IsoRealmsEditor Implement this <---------------------------------- HERE TODAY!
      return id;
 //   } else {
 //     IAction* mConfirmedAction = cConfirmationCallback(mAction);
 //     return cBrowser->getID(mConfirmedAction->getActionType());
//    }
  }
}
