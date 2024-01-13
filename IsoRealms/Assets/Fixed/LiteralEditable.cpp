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
#include "LiteralEditable.h"

namespace IsoRealms {
  LiteralEditable::EditableScreen LiteralEditable::DUMMY;
  
  IEditableScreen* LiteralEditable::createEditableScreen(Project* project) {
    return &DUMMY;
  }

  void LiteralEditable::EditableScreen::contextVisible() {
    // Nothing to do.
  }

  void LiteralEditable::EditableScreen::contextHidden() {
    // Nothing to do.
  }

  void LiteralEditable::EditableScreen::unregisterAssets(IAssetRemover* assets) {
    // Nothing to do.
  }

  void LiteralEditable::EditableScreen::setAppearance(IFont* font, float scale) {
    // Nothing to do.
  }

  IScreen* LiteralEditable::EditableScreen::screen() {
    return nullptr;
  }

  void LiteralEditable::EditableScreen::renderScreen(float scale, float aspectRatio) const {
    // Nothing to do.
  }

  const IFloat* LiteralEditable::EditableScreen::getYaw() const {
    return nullptr;
  }

  const IFloat* LiteralEditable::EditableScreen::getPitch() const {
    return nullptr;
  }

  bool LiteralEditable::EditableScreen::input(sf::Event& event) {
    return false;
  }

  bool LiteralEditable::EditableScreen::renderAssetIcon() const {
    return false;
  }
}
