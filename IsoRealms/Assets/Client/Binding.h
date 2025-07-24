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

#include <functional>

#include "IsoRealms/Assets/Type/IBinding.h"

#include "Asset.h"

namespace IsoRealms {
  class IActionClient;

  class Binding : public Asset<Binding, IBinding, IActionClient> {
    public:
    Binding(IActionClient& owner);
    Binding(IActionClient& owner, const std::string& type);
    std::string getType() const;

    std::string getID() const override;
    void setID(const std::string& id) override;
    bool renderAssetIcon() const override;

    /*********************************************\
     * Implements Asset<IBinding, IActionClient> *
    \*********************************************/
    IBinding* getAsset(IActionClient& owner, JSONObject object);
    IBinding* getAsset(IActionClient& owner, const std::string& id);
    std::vector<std::string> getAvailableClientProviders() const;
    bool renderOtherClientProviderIcon(const std::string& id) const;
    bool hasClientConfiguration() const;
    bool isDefaultConfiguration() const;

    void getTheAssetProperties(IBinding* asset, PropertyMaker& owner) override;

    private:
    std::string cDefType;
    IBindingRegistry* cDefRegistry;
    std::function<void()> cListener;
  };
}
