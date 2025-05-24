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
#include "FloatSwitch.h"

namespace IsoRealms::Basics {
  const std::string FloatSwitch::JSON_VALUE = "value";

  FloatSwitch::FloatSwitch(IProject& project, Basics& basics, IResourceData& data) :
            AssetSwitchWithTransition(project, [this]() {return *cDefFloat;}),
            cDefFloat(project, 0.0f),
            cLuaBinding(project, this) {
  }

  FloatSwitch::FloatSwitch(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            FloatSwitch(project, basics, data) {
    cDefFloat.init(object, JSON_VALUE);
  }

  void FloatSwitch::registerAssets(IAssetRegistry& assets) {
    assets.add(&cLuaBinding, "", "Float Switches");
    assets.add(this, "", "Float Switches");
  }

  void FloatSwitch::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(&cLuaBinding, relinquish);
    assets.remove(this,         relinquish);
  }

  void FloatSwitch::save(JSONObject object, IAssetIdentifier& identifier) const {
    cDefFloat.save(object, JSON_VALUE);
  }

  bool FloatSwitch::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> FloatSwitch::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>("Initial Value", cDefFloat));
    return mProperties;
  }

  float FloatSwitch::getValue() const {
    float mValue = cRuntimeValues.front().getValue()->getValue();
    for (std::vector<Value>::size_type i = 1; i < cRuntimeValues.size(); i++) {
      float mNextValue = cRuntimeValues[i].getValue()->getValue();
      float mNextValueWeight = cRuntimeValues[i].getWeight();
      mValue += (mNextValue - mValue) * mNextValueWeight;
    }
    return mValue;
  }

  bool FloatSwitch::renderAssetIcon() const {
    return false;
  }

  void FloatSwitch::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> FloatSwitch::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool FloatSwitch::isDefaultConfiguration() const {
    return true;
  }
}
