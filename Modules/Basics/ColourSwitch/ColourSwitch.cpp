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
#include "ColourSwitch.h"

namespace IsoRealms::Basics {
  const std::string ColourSwitch::JSON_COLOUR = "colour";

  ColourSwitch::ColourSwitch(IProject* project, Basics* basics) :
            AssetSwitchWithTransition(project, [this]() {return nullptr; /* *cDefColour*/;}),
            cDefColour(project, 0.0f, 0.0f, 0.0f, 0.0f),
            cLuaBinding(project, this) {
  }

  ColourSwitch::ColourSwitch(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data) :
            ColourSwitch(project, basics) {
    cDefColour.init(object, JSON_COLOUR);
  }

  void ColourSwitch::registerAssets(IAssetRegistry* assets) {
    assets->add(&cLuaBinding, "", "Colour Switches");
    assets->add(this, "", "Colour Switches");
  }

  void ColourSwitch::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(&cLuaBinding);
    assets->remove(this);
  }

  void ColourSwitch::save(JSONObject object, IAssetIdentifier* identifier) const {
    cDefColour.save(object, JSON_COLOUR);
  }

  void ColourSwitch::set() const {
    glColor4f(getRed(), getGreen(), getBlue(), getAlpha());
  }

  float ColourSwitch::getRed() const {
    float mValue = cRuntimeValues.front().getValue()->getRed();
    for (std::vector<Value>::size_type i = 1; i < cRuntimeValues.size(); i++) {
      float mNextValue = cRuntimeValues[i].getValue()->getRed();
      float mNextValueWeight = cRuntimeValues[i].getWeight();
      mValue += (mNextValue - mValue) * mNextValueWeight;
    }
    return mValue;
  }

  float ColourSwitch::getGreen() const {
    float mValue = cRuntimeValues.front().getValue()->getGreen();
    for (std::vector<Value>::size_type i = 1; i < cRuntimeValues.size(); i++) {
      float mNextValue = cRuntimeValues[i].getValue()->getGreen();
      float mNextValueWeight = cRuntimeValues[i].getWeight();
      mValue += (mNextValue - mValue) * mNextValueWeight;
    }
    return mValue;
  }

  float ColourSwitch::getBlue() const {
    float mValue = cRuntimeValues.front().getValue()->getBlue();
    for (std::vector<Value>::size_type i = 1; i < cRuntimeValues.size(); i++) {
      float mNextValue = cRuntimeValues[i].getValue()->getBlue();
      float mNextValueWeight = cRuntimeValues[i].getWeight();
      mValue += (mNextValue - mValue) * mNextValueWeight;
    }
    return mValue;
  }

  float ColourSwitch::getAlpha() const {
    float mValue = cRuntimeValues.front().getValue()->getAlpha();
    for (std::vector<Value>::size_type i = 1; i < cRuntimeValues.size(); i++) {
      float mNextValue = cRuntimeValues[i].getValue()->getAlpha();
      float mNextValueWeight = cRuntimeValues[i].getWeight();
      mValue += (mNextValue - mValue) * mNextValueWeight;
    }
    return mValue;
  }

  bool ColourSwitch::renderAssetIcon() const {
    return false;
  }

  void ColourSwitch::saveAsset(JSONObject object) const {
    // Nothing to do.
  }
}
