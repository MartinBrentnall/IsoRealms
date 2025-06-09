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
#include "SimpleString.h"

namespace IsoRealms::Basics {
  const std::string SimpleString::JSON_VALUE = "value";

  const std::string SimpleString::PROPERTY_VALUE = "Initial Value";

  SimpleString::SimpleString(IProject& project, Basics& basics, IResourceData& data) :
            cProjectCallbackManager(project),
            cDefValue(""),
            cRuntimeValue(""),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    cProjectCallbackManager.reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
  
  SimpleString::SimpleString(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            SimpleString(project, basics, data) {
    cRuntimeValue = cDefValue = object.getString(JSON_VALUE);

    project.init([this](IAssets& resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleString::registerAssets(IAssetRegistry& assets) {
    cStateNotifier = assets.add(this, "", "Simple Strings");
    assets.add(&cLuaBinding, "", "Simple Strings");
  }
  
  void SimpleString::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(this,         relinquish);
    assets.remove(&cLuaBinding, relinquish);
    cStateNotifier = nullptr;
  }
  
  void SimpleString::save(JSONObject object, IAssetIdentifier& identifier) const {
    object.addString(JSON_VALUE, cDefValue);
  }

  void SimpleString::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleString::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> SimpleString::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeString>("Initial Value", "TODO", [this]() {return cDefValue;}, [this](const std::string& value) {cDefValue = value; return true;}));
    return mProperties;
  }

  std::string SimpleString::getValue() const {
    return cRuntimeValue;
  }

  bool SimpleString::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleString::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SimpleString::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SimpleString::isDefaultConfiguration() const {
    return true;
  }

  void SimpleString::setValue(const std::string& value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged(this);
  }
}
