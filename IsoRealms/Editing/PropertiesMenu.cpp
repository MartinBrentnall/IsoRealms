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
#include "PropertiesMenu.h"

#include "Configurator.h"

namespace IsoRealms {
  PropertiesMenu::PropertiesMenu(Configurator* parent, IPropertyAppearance* appearance) : AbstractMenu(parent, appearance),
            cEditing(nullptr) {
  }

  void PropertiesMenu::addItem(IProperty* property) {
    cProperties.emplace_back(property);
  }

  void PropertiesMenu::clear() {
    cProperties.clear();
  }

  float PropertiesMenu::getLeftSelectionBoundary(float aspectRatio, unsigned int item) {
    return getLeftSelectionHighlight(aspectRatio, item);
  }

  float PropertiesMenu::getRightSelectionBoundary(float aspectRatio, unsigned int item) {
    return getRightSelectionHighlight(aspectRatio, item);
  }

  float PropertiesMenu::getLeftSelectionHighlight(float aspectRatio, unsigned int item) {
    return cParent->getScreenLeftBorder(aspectRatio) + getMaxLabelWidth() + cIntAppearance->getLabelPropertySpacing();
  }

  float PropertiesMenu::getRightSelectionHighlight(float aspectRatio, unsigned int item) {
    return cParent->getScreenLeftBorder(aspectRatio) + getMaxLabelWidth() + cIntAppearance->getLabelPropertySpacing() + cProperties[item]->getWidth(cIntAppearance);
  }

  unsigned int PropertiesMenu::getItemCount() {
    return static_cast<int>(cProperties.size());
  }

  void PropertiesMenu::renderOverlay() {
    for (unsigned int i = 0; i < cClosingProperties.size(); i++) {
      cClosingProperties[i]->renderEditing(cIntAppearance);
    }

    if (cEditing != nullptr) {
      glColor3f(1.0f, 1.0f, 1.0f);
      cEditing->renderEditing(cIntAppearance);
    }
  }

  void PropertiesMenu::renderItem(float aspectRatio, unsigned int item, float x) {
    float mBorderPadding = cIntAppearance->getSelectionHighlightHeight() * 0.5f;
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string mPropertyName = cProperties[item]->getPropertyName() + ":";
    glPushMatrix();
    glTranslatef(cParent->getScreenLeftBorder(aspectRatio) + mBorderPadding, cParent->getTopIconPosition() - mBorderPadding * 2.0f, 0.0f);
    cIntAppearance->print(mPropertyName.c_str(), item, 0.0f);
    glPopMatrix();
//     if (cEditing != cProperties[item]) {
//       glTranslatef(x, cParent->getTopIconPosition(), 0.0f);
//       cProperties[item]->render(cIntAppearance);
//     }
  }

  void PropertiesMenu::updateItems(unsigned int milliseconds) {
    if (cEditing != nullptr) {
      cEditing->updateEditing(milliseconds);
    }

    std::stack<unsigned int> mClosedIndices;
    for (unsigned int i = 0; i < cClosingProperties.size(); i++) {
      if (cClosingProperties[i]->updateEditing(milliseconds)) {
        mClosedIndices.push(i);
      }
    }

    while (!mClosedIndices.empty()) {
      unsigned int mClosedIndex = mClosedIndices.top();
      cClosingProperties.erase(cClosingProperties.begin() + mClosedIndex);
      mClosedIndices.pop();
    }
  }

  bool PropertiesMenu::isMenuInputLocked() {
    return cEditing != nullptr;
  }

  void PropertiesMenu::edit(unsigned int item) {
    if (cEditing == nullptr) {
      IEditable* mEditable = cProperties[item]->edit(nullptr);
//              IEditable* mEditable = cProperties[item]->edit(cParent->getRuntimeProject());
      if (mEditable != nullptr) {
        // TODO: This should probably invoke an action to handle the editable thing.
//                 cParent->cRuntimeMenus.push_back(new EmptyScreen());
//                 cParent->cRuntimeMode = EDITOR_SCREEN;
//                 if (cParent->cRuntimeEditorScreen != nullptr) {
//                   cParent->cRuntimeEditorScreen->contextHidden();
//                   cParent->cRuntimeProject->remove(cParent->cRuntimeEditorScreen);
//                   cParent->cRuntimeEditor->destroyEditableScreen(cParent->cRuntimeEditorScreen, cParent->getRuntimeProject());
//                 }
//                 cParent->cRuntimeEditor = mEditable;
//                 cParent->cRuntimeEditorScreen = cParent->cRuntimeEditor->createEditableScreen();
//                 if (cParent->cRuntimeEditorScreen != nullptr) {
//                   cParent->cRuntimeProject->add(cParent->cRuntimeEditorScreen, "Editor", "External");
//                   cParent->cRuntimeEditorScreen->notifyVisible();
//                 }
      } else {
        cEditing = cProperties[item];
      }
    }
  }

  bool PropertiesMenu::input(unsigned int item, ConfiguratorSignalID id) {
    if (cEditing != nullptr) {
      if (cEditing->input(id)) {
        cEditing->close();
        cClosingProperties.push_back(cEditing);
        cEditing = nullptr;
      }
      return true;
    } else switch (id) {
      case ConfiguratorSignalID::CONFIRM: edit(item); return true;
      default:                                        break;
    }
    return false;
  }

  float PropertiesMenu::getMaxLabelWidth() const {
    float mMaxWidth = 0.0f;
    for (IProperty* mProperty : cProperties) {
      std::string mPropertyName = mProperty->getPropertyName() + ":";
      mMaxWidth = std::max(mMaxWidth, cIntAppearance->getWidth(mPropertyName));
    }
    return mMaxWidth;
  }

  void PropertiesMenu::refresh() {
    // Nothing to do.
  }
}
