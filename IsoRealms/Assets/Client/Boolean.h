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

#include "IsoRealms/Assets/Type/IBoolean.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/JSONDocument.h"

#include "Asset.h"

namespace IsoRealms {
  class Boolean : public Asset<Boolean, IBoolean, IResourceData>,
                  public IStateListener<IBoolean*> {
    public:
    Boolean(IResourceData& owner, bool defaultValue = false, std::function<void(bool)> listener = nullptr);

    /*********************************************\
     * Implements Asset<IBoolean, IResourceData> *
    \*********************************************/
    IBoolean* createLiteralAsset(IResourceData& owner);
    IBoolean* getAsset(IResourceData& owner, JSONObject object);
    IBoolean* getAsset(IResourceData& owner, const std::string& id);
    std::vector<std::string> getAvailableProviders() const;
    bool renderOtherProviderIcon(const std::string& id) const;
    bool hasConfiguration() const;
    bool isDefaultConfiguration() const;

    /****************************************\
     * Implements IStateListener<IBoolean*> *
    \****************************************/
    void stateChanged(IBoolean* asset) override;

    private:
    bool cDefaultValue;
    std::function<void(bool)> cListener;
  };
}
