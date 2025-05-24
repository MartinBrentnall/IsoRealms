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

  ColourSwitch::ColourSwitch(IProject& project, Basics& basics, IResourceData& data) :
            AssetSwitchWithTransition(project, [this]() {return *cDefColour;}),
            cDefColour(project, 0.0f, 0.0f, 0.0f, 0.0f),
            cLuaBinding(project, this) {
  }

  ColourSwitch::ColourSwitch(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            ColourSwitch(project, basics, data) {
    cDefColour.init(object, JSON_COLOUR);
  }

  void ColourSwitch::registerAssets(IAssetRegistry& assets) {
    assets.add(&cLuaBinding, "", "Scriptable Colours");
    assets.add(this, "", "Scriptable Colours");
  }

  void ColourSwitch::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(&cLuaBinding, relinquish);
  }

  void ColourSwitch::save(JSONObject object, IAssetIdentifier& identifier) const {
    cDefColour.save(object, JSON_COLOUR);
  }

  bool ColourSwitch::renderIcon() {
    return renderAssetIcon();
  }

  std::vector<std::unique_ptr<IProperty>> ColourSwitch::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Initial Value", cDefColour));
    return mProperties;
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
    return cDefColour.renderAssetIcon();
  }

  void ColourSwitch::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ColourSwitch::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ColourSwitch::isDefaultConfiguration() const {
    return true;
  }
}
