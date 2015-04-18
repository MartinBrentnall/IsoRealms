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
#include "FontIcon.h"

FontIcon::FontIcon(IResourceBrowser<IFont>* browser, IFont* font) : Icon<IFont>(browser, font) {
}

void FontIcon::renderIcon() {
  IFont* mFont = getResource();
  float mLeft = getLeft();
  float mBottom = getBottom();
  glColor3f(1.0f, 1.0f, 1.0f);
  float mFontSize = LookAndFeel::getDefaultFontSize();
  mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, 0, getResourceLocation().c_str());
}

float FontIcon::getWidth() {
  return 0.4f;
}

float FontIcon::getHeight() {
  return 0.1f;
}
