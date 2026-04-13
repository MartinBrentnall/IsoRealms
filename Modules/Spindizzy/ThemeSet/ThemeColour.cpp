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
#include "ThemeColour.h" 

#include "Modules/Spindizzy/Spindizzy.h"

#include "ThemeSet.h"

namespace IsoRealms::Spindizzy {
  ThemeColour::ThemeColour(ThemeSet& parent) :
            cParent(parent),
            cColour(nullptr),
            cLuaBinding(parent.getSpindizzy().getProject().getLuaState(), this) {
  }

  void ThemeColour::registerAssets(ResourceAssetRegistry& assets, const std::string& id) {
    assets.add<IColour>(this, id, "Colours from Spindizzy Themes"); // TODO: Localize this.
    assets.add<IBinding>(&cLuaBinding, "ThemeColour/" + id, "Spindizzy/Theme Colours");
  }
  
  void ThemeColour::set(IColour* colour) {
    cColour = colour;
  }

  float ThemeColour::getRed() const {
    return cColour != nullptr ? cColour->getRed() : 0.0f;
  }

  float ThemeColour::getGreen() const {
    return cColour != nullptr ? cColour->getGreen() : 0.0f;
  }

  float ThemeColour::getBlue() const {
    return cColour != nullptr ? cColour->getBlue() : 0.0f;
  }

  float ThemeColour::getAlpha() const {
    return cColour != nullptr ? cColour->getAlpha() : 0.0f;
  }

  void ThemeColour::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void ThemeColour::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool ThemeColour::isDefaultConfiguration() const {
    return true;
  }

  void ThemeColour::set() const {
    if (cColour != nullptr) {
      cColour->set();
    } else {
      glColor3f(1.0f, 0.0f, 1.0f);
    }
  }
}
