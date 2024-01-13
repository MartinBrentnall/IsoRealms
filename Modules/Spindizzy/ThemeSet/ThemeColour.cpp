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
#include "ThemeColour.h" 

namespace IsoRealms::Spindizzy {
  ThemeColour::ThemeColour(IProject* project, IIconAnimator* animator) :
            cLuaBinding(project, this) {
    cAnimator = animator;
    cColour = nullptr;
  }

  void ThemeColour::registerAssets(IAssetRegistry* assets, const std::string& id) {
    assets->add(this, id, "Spindizzy Themes");
    assets->add(&cLuaBinding, "ThemeColour/" + id, "Spindizzy Themes");
  }
  
  void ThemeColour::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
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

  void ThemeColour::set() const {
    if (cColour != nullptr) {
      cColour->set();
    } else {
      glColor3f(1.0f, 0.0f, 1.0f);
    }
  }
}
