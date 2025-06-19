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

#include <functional>

#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/JSONDocument.h"

#include "Asset.h"

namespace IsoRealms {
  class Binding : public Asset<IBinding, IResourceData> {
    public:
    Binding(IResourceData& owner, IBindingRegistry* registry, std::function<void()> listener = nullptr);
    Binding(IResourceData& owner, IBindingRegistry* registry, const std::string& type, std::function<void()> listener = nullptr);
    std::string getType() const;

    std::string getID() const override;
    void setID(const std::string& id) override;
    bool renderAssetIcon() const override;

    /*********************************************\
     * Implements Asset<IBinding, IResourceData> *
    \*********************************************/
    IBinding* createLiteralAsset(IResourceData& owner) override;
    IBinding* getAsset(IResourceData& owner, JSONObject object) override;
    IBinding* getAsset(IResourceData& owner, const std::string& id) override;
    std::vector<std::string> getAvailableProviders() const override;
    bool renderOtherProviderIcon(const std::string& id) const override;
    bool hasConfiguration() const override;
    bool isDefaultConfiguration() const override;
    void stateChanged(IBinding* asset) override;
    std::vector<std::unique_ptr<IProperty>> getTheAssetProperties(IBinding* asset) override;

    private:
    std::string cDefType;
    IBindingRegistry* cDefRegistry;
    std::function<void()> cListener;
  };
}
