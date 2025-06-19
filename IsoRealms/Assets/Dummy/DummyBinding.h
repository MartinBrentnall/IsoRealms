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
#pragma once

#include "IsoRealms/Assets/Type/IBinding.h"

namespace IsoRealms {
  class IResourceData;

  class DummyBinding : public IBinding {
    public:
    DummyBinding(IResourceData& owner);

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
    std::vector<std::unique_ptr<IProperty>> getWrappedProperties() override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;
  };
}
