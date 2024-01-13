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

namespace IsoRealms {
  
  /**
   * A simple fixed String asset implementation.
   */
  class LiteralString : public IString {
    private:
    std::string cValue; /// The value of this Boolean.
    
    public:
      
    /**
     * Construct the String with the specified value.
     * 
     * @param value Value of the String.
     */
    LiteralString(const std::string&);
      
    void setValue(const std::string&);
    
    /**********************\
     * Implements IString *
    \**********************/
    std::string getValue() const override;
    bool renderAssetIcon() const override;
  };
}
