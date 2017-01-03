/*
 * Copyright 2016 Martin Brentnall
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
#include "BooleanIcon.h"

BooleanIcon::BooleanIcon(IResourceBrowser<IBoolean>* browser, IBoolean* boolean) : Icon<IBoolean>(browser, boolean) {
  FixedSizeLayout* mFixedSizeLayout = new FixedSizeLayout(true, true, this, &cCheckBox);
  cCheckBox.setBoundsCalculator(mFixedSizeLayout);
  cCheckBox.addValueListener(this);
}

bool BooleanIcon::input(SDL_Event& event) {
  return cCheckBox.input(event) || Icon<IBoolean>::input(event);
}

void BooleanIcon::renderIcon() {
  IBoolean* mBoolean = getResource();
  cCheckBox.setValue(mBoolean->getValue());
  cCheckBox.render();
  float mLeft = getLeft();
  float mBottom = getBottom();
  glColor3f(1.0f, 1.0f, 1.0f);
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  mFont->print(mLeft + 0.06f * mAspectRatio, mBottom + 0.01f, mFontSize, IFont::LEFT, getResourceLocation().c_str());
}

float BooleanIcon::getWidth() {
  return 0.4f;
}

float BooleanIcon::getHeight() {
  return 0.05f;
}

float BooleanIcon::getX() {
  return getLeft();
}

float BooleanIcon::getY() {
  return getTop();
}

void BooleanIcon::valueChanged(IValueComponent<bool>* component, bool value) {
  IBoolean* mBoolean = getResource();
  mBoolean->setInitialValue(value);
}

