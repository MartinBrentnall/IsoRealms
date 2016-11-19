/*
 * Copyright 2015 Martin Brentnall
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
#include "DockedDialog.h"

DockedDialog::DockedDialog(Dialog* dialog, AbstractRectangularComponent* icon, float preferredSize) {
  cDialog = dialog;
  cIcon = icon;
  cPreferredSize = preferredSize;
}

bool DockedDialog::input(SDL_Event& event) {
  return cDialog->input(event);
}

void DockedDialog::update(unsigned int milliseconds) {
  cDialog->update(milliseconds);
  cIcon->update(milliseconds);
}

void DockedDialog::render() {
  cDialog->render();
}

void DockedDialog::renderIcon() {
  cIcon->render();
}

bool DockedDialog::contains(float x, float y) {
  return cDialog->contains(x, y);
}

float DockedDialog::getHeight() {
  return cDialog->getHeight();
}

float DockedDialog::getWidth() {
  return cDialog->getWidth();
}

float DockedDialog::getPreferredSize() {
  return cPreferredSize;
}

void DockedDialog::updatePreferredWidth() {
  cPreferredSize = getWidth();
}

void DockedDialog::setSize(float left, float bottom, float right, float top) {
  cDialog->setSize(left, bottom, right, top);
}

