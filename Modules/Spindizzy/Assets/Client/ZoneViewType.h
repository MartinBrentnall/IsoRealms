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

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"

#include "Modules/Spindizzy/Assets/Type/IZoneViewType.h"

namespace IsoRealms::Spindizzy {
  class WorldView;
  class Spindizzy;
  
  class ZoneViewType : public IAssetUser<IZoneViewType> {
    public:
    ZoneViewType(Spindizzy* spindizzy);

    void init(DOMNode& node, WorldView* owner);
    void save(DOMNodeWriter* node, const std::string& attribute) const;

    std::string get() const;
    void set(DOMNode& node, WorldView* owner);
    IZoneViewType* operator->() const {
      return cZoneViewType;
    }

    IZoneViewType* operator*() const {
      return cZoneViewType;
    }

    /****************************************\
     * Implements IAssetUser<IZoneViewType> *
    \****************************************/
    void relinquish(IZoneViewType* asset) override;

    virtual ~ZoneViewType();

    private:
    Spindizzy* cSpindizzy;
    IZoneViewType* cZoneViewType;

    ZoneViewType(ZoneViewType const& ZoneViewType) = delete;
    ZoneViewType& operator=(ZoneViewType const& ZoneViewType) = delete;
  };
}
