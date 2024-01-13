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

#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"

#include "Modules/Spindizzy/Assets/Type/IPhysicalObjectType.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  class PhysicalObjectType : public IAssetUser<IPhysicalObjectType> {
    public:
    
    PhysicalObjectType(Spindizzy& spindizzy);

    void init(DOMNode& node);
    void save(DOMNodeWriter* node, const std::string& attribute) const;

    std::string get() const;
    void set(DOMNode& node);
    
    IPhysicalObjectType* operator->() const {
      return cPhysicalObjectType;
    }

    IPhysicalObjectType* operator*() const {
      return cPhysicalObjectType;
    }

    IBinding* getBinding(const std::string& id) const;

    /**********************************************\
     * Implements IAssetUser<IPhysicalObjectType> *
    \**********************************************/
    void relinquish(IPhysicalObjectType* asset) override;

    virtual ~PhysicalObjectType();

    private:
    Spindizzy& cSpindizzy;
    IPhysicalObjectType* cPhysicalObjectType;
  };
}
