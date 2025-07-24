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
#pragma once

#include "IsoRealms/Assets/Type/IBinding.h"

namespace IsoRealms {
  class IActionClient;

  class DummyBinding : public IBinding {
    public:
    DummyBinding(IActionClient& owner);

    /***********************\
     * Implements IBinding *
    \***********************/
    void bind(const std::string& function) const override;
    std::vector<std::string> getAvailableProviders() const override;
    bool renderProviderIcon(const std::string& id) const override;
    bool renderWrappedIcon() const override;
    bool isConfigurable() const override;
    std::string getID() const override;
    void set(const std::string& id) override;
    void getWrappedProperties(PropertyMaker& owner) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;
  };
}
