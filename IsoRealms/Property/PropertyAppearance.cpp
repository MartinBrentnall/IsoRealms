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
#include "PropertyAppearance.h"

namespace IsoRealms {
  void PropertyAppearance::set(IFont* font, float scale) {
    cDefFont = font;
    cDefScale = scale;
  }

  void PropertyAppearance::print(const std::string& value, const float lineOffset, float xOffset) const {
    std::cout << "PRINTING " << value << " AT " << ((-(lineOffset + 1.5f) * getLineHeight()) + -cDefScale * 0.6f) << std::endl;
    cDefFont->print(xOffset, (-(lineOffset + 1.5f) * getLineHeight()) + -cDefScale * 0.6f, cDefScale, IFont::Alignment::LEFT, value);
  }

  void PropertyAppearance::print(float x, float y, float scale, IFont::Alignment alignment, const std::string& value) const {
    cDefFont->print(x, y, scale, alignment, value);
  }

  float PropertyAppearance::getWidth(const std::string& value) const {
    return cDefFont->getWidth(cDefScale, value);
  }

  float PropertyAppearance::getIconWidth() const {
    return cDefScale * 2.0f;
  }

  float PropertyAppearance::getLineHeight() const {
    return cDefScale * 2.5f;
  }

  float PropertyAppearance::getSelectionHighlightHeight() const {
    return cDefScale * 2.4f;
  }

  float PropertyAppearance::getIconTextSpacing() const {
    return cDefScale * 0.8f;
  }

  float PropertyAppearance::getLabelPropertySpacing() const {
    return cDefScale * 0.0f;
  }

  void PropertyAppearance::transformForIcon(const float lineOffset) const {
    glTranslatef(getIconWidth() * 0.5f, -(lineOffset + 1.5f) * getLineHeight(), 0.0f);
    glScalef(cDefScale, cDefScale, cDefScale);
  }

  float PropertyAppearance::getY(const float lineOffset) const {
    return -(lineOffset + 1.5f) * getLineHeight();
  }

  float PropertyAppearance::getScale() const {
    return cDefScale;
  }
}
