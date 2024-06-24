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
#include "Configurator.h"

namespace IsoRealms {
  const LiteralColour Configurator::COLOUR_TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);
  const float Configurator::MENU_BOUNDARY_BOTTOM = -0.5f;
  const float Configurator::MENU_BOUNDARY_TOP = 0.5f;
  const float Configurator::SCREEN_BORDER_SIZE = 0.05f;

//   void Configurator::closeAllMenus() {
//     while (!cRuntimeMenus.empty()) {
// //       TODO: Need to do this later because it might delete a menu item we're currently inside!
//       cRuntimeMenus.pop_back();
//     }
//   }

  Configurator::Configurator(IUIConfiguration* editor) :
            cEditor(editor) {
  }

  void Configurator::update(unsigned int milliseconds) {
    for (unsigned int i = 0; i < cRuntimeMenus.size(); ++i) {
      cRuntimeMenus[i]->update(milliseconds, i == cRuntimeMenus.size() - 1);
    }

    std::stack<unsigned int> mClosedIndices;
    for (unsigned int i = 0; i < cRuntimeClosedMenus.size(); ++i) {
      if (cRuntimeClosedMenus[i]->updateForClosure(milliseconds)) {
        mClosedIndices.push(i);
      }
    }

    while (!mClosedIndices.empty()) {
      unsigned int mClosedIndex = mClosedIndices.top();
      cRuntimeClosedMenus.erase(cRuntimeClosedMenus.begin() + mClosedIndex);
      mClosedIndices.pop();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  const IColour* Configurator::getSelectionHighlight() const {
    return cEditor->getSelectionColour();
  }

  const IColour* Configurator::getSelectionLocked() const {
    return cEditor->getLockedColour();
  }

  IFont* Configurator::getFont() const {
    return cEditor->getFont();
  }

  float Configurator::getScreenLeftBorder(float aspectRatio) const {
    return -1.0f * aspectRatio + SCREEN_BORDER_SIZE;
  }

  float Configurator::getScreenTopBorder() const {
    return 1.0f - SCREEN_BORDER_SIZE;
  }

  float Configurator::getTopIconPosition() const {
    return getScreenTopBorder();// - getIconHeight() * 0.5f;
  }

//   float Configurator::getIconHeight() const {
//     return getMenuScale() * 2.0f;
//   }

  float Configurator::getLeftIconPosition(float aspectRatio) const {
    return getScreenLeftBorder(aspectRatio);// + getIconWidth() * 0.5f;
  }

  float Configurator::getMenuBoundaryBottom() const {
    return MENU_BOUNDARY_BOTTOM;
  }

  float Configurator::getMenuBoundaryTop() const {
    return MENU_BOUNDARY_TOP;
  }

//   float Configurator::getExtraActionSpacing() const {
//     return getMenuScale() * 6.0f;
//   }

  void Configurator::render(float aspectRatio) const {
    glEnable(GL_BLEND);
    for (IConfiguratorScreen* mProjectMenu : cRuntimeMenus) {
      mProjectMenu->render(aspectRatio);
    }
    for (IConfiguratorScreen* mProjectMenu : cRuntimeClosedMenus) {
      mProjectMenu->render(aspectRatio);
    }
    glDisable(GL_BLEND);
  }

  void Configurator::open(IConfiguratorScreen* screen) {
    cRuntimeMenus.push_back(screen);
  }

  void Configurator::input(ConfiguratorSignalID id) {
    if (!cRuntimeMenus.empty()) {
      cRuntimeMenus.back()->input(id);
    }
  }

  void Configurator::closeProjectMenu() {
    cRuntimeClosedMenus.push_back(cRuntimeMenus[cRuntimeMenus.size() - 1]);
    cRuntimeMenus.pop_back();
    if (!cRuntimeMenus.empty()) {
      cRuntimeMenus[cRuntimeMenus.size() - 1]->refresh();
    } else {
      cEditor->exit();
    }
  }
}
