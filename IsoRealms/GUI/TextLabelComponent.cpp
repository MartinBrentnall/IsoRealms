/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "TextLabelComponent.h"

TextLabelComponent::TextLabelComponent(std::string label, float padding) {
  cLabel = label;
  cPadding = padding;
}

void TextLabelComponent::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  glColor3f(1.0f, 1.0f, 1.0f);
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  mFont->print(mLeft + cPadding / 2.0f, mBottom + 0.01f, mFontSize, 0, cLabel.c_str());
}

void TextLabelComponent::update(unsigned int milliseconds) {
}

bool TextLabelComponent::input(SDL_Event& event) {
  return false;
}

bool TextLabelComponent::contains(float, float) {
  return false;
}

float TextLabelComponent::getWidth() {
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  return mFont->getWidth(mFontSize, cLabel.c_str()) + cPadding;
}

float TextLabelComponent::getHeight() {
  return 0.05f;
}

// TODO: Consider override contains() method to prevent this component from taking focus?
