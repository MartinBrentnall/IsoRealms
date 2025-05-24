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

#include "IsoRealms/Assets/Client/Asset.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/JSONDocument.h"

#include "Modules/UI/Assets/Type/ILayoutOffset.h"
#include "Modules/UI/IUI.h"

namespace IsoRealms::UI {
  class LayoutOffset : public Asset<ILayoutOffset, IUI> {
    public:
    LayoutOffset(IUI& ui, LayoutComponentEdge& owner);
    LayoutOffset(const LayoutOffset& offset);

    /****************************************\
     * Implements Asset<ILayoutOffset, IUI> *
    \****************************************/
    ILayoutOffset* createLiteralAsset(IUI& ui) override;
    ILayoutOffset* getAsset(IUI& ui, JSONObject object) override;
    ILayoutOffset* getAsset(IUI& ui, const std::string& id) override;
    std::vector<std::string> getAvailableProviders() const override;
    bool renderOtherProviderIcon(const std::string& id) const override;
    bool hasConfiguration() const override;
    bool isDefaultConfiguration() const override;

    private:
    LayoutComponentEdge& cOwner;
  };
}
