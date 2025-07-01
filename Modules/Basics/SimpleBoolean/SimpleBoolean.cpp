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
#include "SimpleBoolean.h"

namespace IsoRealms::Basics {
  const std::string SimpleBoolean::JSON_VALUE = "value";

  const std::string SimpleBoolean::PROPERTY_VALUE = "Initial Value";
  
  SimpleBoolean::SimpleBoolean(IProject& project, Basics& basics, IResourceData& data) :
            cProjectCallbackManager(project),
            cDefValue(false),
            cRuntimeValue(false),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    cProjectCallbackManager.reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
  
  SimpleBoolean::SimpleBoolean(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            SimpleBoolean(project, basics, data) {
    cRuntimeValue = cDefValue = object.getBoolean(JSON_VALUE);

    project.init([this](IAssets& resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleBoolean::registerAssets(IAssetRegistry& assets) {
    cStateNotifier = assets.add(this, "", "Simple Booleans");
    assets.add(&cLuaBinding, "", "Simple Booleans");
  }

  void SimpleBoolean::save(JSONObject object) const {
    object.addBoolean(JSON_VALUE, cDefValue);
  }

  void SimpleBoolean::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleBoolean::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> SimpleBoolean::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("Value", "TODO", [this]() {return cDefValue;}, [this](bool value) {cDefValue = value;}, owner.getProject()));
    return mProperties;
  }

  bool SimpleBoolean::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleBoolean::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleBoolean::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SimpleBoolean::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SimpleBoolean::isDefaultConfiguration() const {
    return true;
  }

  void SimpleBoolean::setValue(bool value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
