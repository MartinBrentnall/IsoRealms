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
#include "LiteralColour.h"

#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  LiteralColour::LiteralColour() :
            cRed(  0.0f),
            cGreen(0.0f),
            cBlue( 0.0f),
            cAlpha(0.0f) {
  }

  LiteralColour::LiteralColour(const IColour& colour, const float intensity) :
            cRed(  colour.getRed()   * intensity),
            cGreen(colour.getGreen() * intensity),
            cBlue( colour.getBlue()  * intensity),
            cAlpha(colour.getAlpha()) {
  }

  LiteralColour::LiteralColour(const float red, const float green, const float blue, const float alpha) :
            cRed(red),
            cGreen(green),
            cBlue(blue),
            cAlpha(alpha) {
  }

  LiteralColour::LiteralColour(const IColour& a, const IColour& b, const float weight) :
            cRed(  a.getRed()   + (b.getRed()   - a.getRed())   * weight),
            cGreen(a.getGreen() + (b.getGreen() - a.getGreen()) * weight),
            cBlue( a.getBlue()  + (b.getBlue()  - a.getBlue())  * weight),
            cAlpha(a.getAlpha() + (b.getAlpha() - a.getAlpha()) * weight) {
  }

  bool LiteralColour::operator==(const LiteralColour& colour) const {
    return cRed == colour.cRed && cGreen == colour.cGreen && cBlue == colour.cBlue && cAlpha == colour.cAlpha;
  }

  bool LiteralColour::operator!=(const LiteralColour& colour) const {
    return !operator==(colour);
  }

  void LiteralColour::set() const {
    glColor4f(cRed, cGreen, cBlue, cAlpha);
  }

  float LiteralColour::getRed() const {
    return cRed;
  }

  float LiteralColour::getGreen() const {
    return cGreen;
  }

  float LiteralColour::getBlue() const {
    return cBlue;
  }

  float LiteralColour::getAlpha() const {
    return cAlpha;
  }

  void LiteralColour::saveAsset(DOMNodeWriter* node) const {
    node->addAttribute("value", Utils::toString(cRed) + " " + Utils::toString(cGreen) + " " + Utils::toString(cBlue) + " " + Utils::toString(cAlpha));
  }
}
