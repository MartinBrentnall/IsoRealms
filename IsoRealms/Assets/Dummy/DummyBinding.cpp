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
#include "DummyBinding.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyBinding::DummyBinding(IResourceData& owner) {
    // Nothing to do.
  }

  void DummyBinding::bind(const std::string& function) const {
    // Nothing to do.
  }

  std::vector<std::string> DummyBinding::getAvailableProviders() const {
    return std::vector<std::string>();
  }

  bool DummyBinding::renderProviderIcon(const std::string& id) const {
    return false;
  }

  bool DummyBinding::renderWrappedIcon() const {
    return false;
  }

  bool DummyBinding::isConfigurable() const {
    return false;
  }

  std::string DummyBinding::getID() const {
    return "";
  }

  void DummyBinding::set(const std::string& id) {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> DummyBinding::getWrappedProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool DummyBinding::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void DummyBinding::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> DummyBinding::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool DummyBinding::isDefaultConfiguration() const {
    return true;
  }
}
