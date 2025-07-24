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
#include "DummyScreen.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyScreen::DummyScreen(IResourceData& owner) {
    // Nothing to do.
  }

  void DummyScreen::renderScreen(float scale, float aspectRatio) const {
    // Nothing to do
  }

  bool DummyScreen::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void DummyScreen::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void DummyScreen::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool DummyScreen::isDefaultConfiguration() const {
    return true;
  }
}
