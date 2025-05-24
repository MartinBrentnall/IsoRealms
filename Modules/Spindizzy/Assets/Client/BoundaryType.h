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

#include "IsoRealms/Assets/Client/Asset.h"
#include "IsoRealms/Persistence/JSONDocument.h"

#include "Modules/Spindizzy/Assets/Type/IBoundaryType.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  
  class BoundaryType : public Asset<IBoundaryType, Spindizzy> {
    public:
    BoundaryType(Spindizzy& spindizzy);

    IBinding* getBoundaryBinding(const std::string& id) const;
    std::string getBoundaryBindingID(const IBinding* binding) const;

    /**********************************************\
     * Implements Asset<IBoundaryType, Spindizzy> *
    \**********************************************/
    IBoundaryType* createLiteralAsset(Spindizzy& spindizzy) override;
    IBoundaryType* getAsset(Spindizzy& spindizzy, JSONObject object) override;
    IBoundaryType* getAsset(Spindizzy& spindizzy, const std::string& id) override;
    std::vector<std::string> getAvailableProviders() const override;
    bool renderOtherProviderIcon(const std::string& id) const override;
    bool hasConfiguration() const override;
    bool isDefaultConfiguration() const override;
  };
}
