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
#include "SoundIcon.h"

SoundIcon::SoundIcon(IResourceBrowser<ISound>* browser, ISound* sound) : Icon<ISound>(browser, sound) {
}

void SoundIcon::renderIcon() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  glColor3f(1.0f, 1.0f, 1.0f);
  mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, IFont::LEFT, getResourceLocation().c_str());
}

void SoundIcon::iconSelected() {
  ISound* mSound = getResource();
  mSound->play();
}

float SoundIcon::getWidth() {
  return 0.66f;
}

float SoundIcon::getHeight() {
  return 0.05f;
}
