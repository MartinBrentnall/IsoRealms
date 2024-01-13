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

#include "IsoRealms/Assets/Type/IString.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  template <class TYPE> class PrimToString : public IString {
    public:
    PrimToString(TYPE* value) :
              cValue(value) {
    }

    /**********************\
     * Implements IString *
    \**********************/
    std::string getValue() const override {
      return Utils::toString(cValue->getValue());
    }

    bool renderAssetIcon() const override {
      return false;
    }

    private:
    TYPE* cValue;
  };
}

