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

#include "IsoRealms/Property/IProperty.h"
#include "IsoRealms/Literals.h"

namespace IsoRealms::Spindizzy {
  class Zone;

  class IWorldObject {
    public:
    virtual bool contains(const LiteralVertex& location) const = 0;
    virtual void renderSelectionHighlight() const = 0;
    virtual void remove() = 0;
    virtual std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyAppearance* appearance) = 0;
    virtual std::string getTypeName() const = 0;
    virtual Zone& getObjectZone() = 0;

    virtual ~IWorldObject() {}
  };
}
