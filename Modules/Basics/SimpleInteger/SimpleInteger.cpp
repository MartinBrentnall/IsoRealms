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
#include "SimpleInteger.h"

namespace IsoRealms::Basics {
  const std::string SimpleInteger::JSON_VALUE = "value";

  const std::string SimpleInteger::PROPERTY_VALUE = "Initial Value";

  SimpleInteger::SimpleInteger(IProject& project, Basics& basics, IResourceData& data) :
            cProjectCallbackManager(project),
            cDefValue(0),
            cRuntimeValue(0),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    cProjectCallbackManager.reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
  
  SimpleInteger::SimpleInteger(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            SimpleInteger(project, basics, data) {
    cRuntimeValue = cDefValue = object.getInteger(JSON_VALUE);

    project.init([this](IAssets& resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleInteger::registerAssets(IAssetRegistry& assets) {
    cStateNotifier = assets.add(this, "", "Simple Integers");
    assets.add(&cLuaBinding, "", "Simple Integers");
  }
  
  void SimpleInteger::save(JSONObject object) const {
    object.addInteger(JSON_VALUE, cDefValue);
  }

  void SimpleInteger::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleInteger::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> SimpleInteger::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>("Value", "TODO", [this]() {return cDefValue;}, [this](int value) {cDefValue = value; return true;}));
    return mProperties;
  }

  int SimpleInteger::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleInteger::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleInteger::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SimpleInteger::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SimpleInteger::isDefaultConfiguration() const {
    return true;
  }

  void SimpleInteger::setValue(int value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
