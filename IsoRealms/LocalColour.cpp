/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "LocalColour.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/PropertyMaker.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  LocalColour::LocalColour() :
            cRed(  0.0f),
            cGreen(0.0f),
            cBlue( 0.0f),
            cAlpha(1.0f) {
  }

  LocalColour::LocalColour(const IColour& colour, const float intensity) :
            cRed(  colour.getRed()   * intensity),
            cGreen(colour.getGreen() * intensity),
            cBlue( colour.getBlue()  * intensity),
            cAlpha(colour.getAlpha()) {
  }

  LocalColour::LocalColour(const float red, const float green, const float blue, const float alpha) :
            cRed(red),
            cGreen(green),
            cBlue(blue),
            cAlpha(alpha) {
  }

  LocalColour::LocalColour(const IColour& a, const IColour& b, const float weight) :
            cRed(  a.getRed()   + (b.getRed()   - a.getRed())   * weight),
            cGreen(a.getGreen() + (b.getGreen() - a.getGreen()) * weight),
            cBlue( a.getBlue()  + (b.getBlue()  - a.getBlue())  * weight),
            cAlpha(a.getAlpha() + (b.getAlpha() - a.getAlpha()) * weight) {
  }

  bool LocalColour::operator==(const LocalColour& colour) const {
    return cRed == colour.cRed && cGreen == colour.cGreen && cBlue == colour.cBlue && cAlpha == colour.cAlpha;
  }

  bool LocalColour::operator!=(const LocalColour& colour) const {
    return !operator==(colour);
  }

  void LocalColour::set() const {
    glColor4f(cRed, cGreen, cBlue, cAlpha);
  }

  float LocalColour::getRed() const {
    return cRed;
  }

  float LocalColour::getGreen() const {
    return cGreen;
  }

  float LocalColour::getBlue() const {
    return cBlue;
  }

  float LocalColour::getAlpha() const {
    return cAlpha;
  }

  void LocalColour::saveAsset(JSONObject object) const {
    // Not supported.
  }

  void LocalColour::getAssetProperties(PropertyMaker& owner) {
    // Not supported.
  }

  bool LocalColour::isDefaultConfiguration() const {
    return true;
  }
}
