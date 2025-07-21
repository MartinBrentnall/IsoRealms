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

#include "IsoRealms/Assets/Type/IInteger.h"

namespace IsoRealms {
  
  /**
   * Simple general immutable integer asset implementation.
   */
  class LiteralInteger : public IInteger {
    private:
    static const std::string JSON_VALUE;

    int cValue; /// Integer value.
    
    public:
      
    /**
     * Construct an integer with the specified value.
     * 
     * @param value Value of this integer.
     */
    LiteralInteger(const int value = 0);
      
    /***********************\
     * Implements IInteger *
    \***********************/
    int getValue() const override;

    /***********************************\
     * Implements IAsset from IInteger *
    \***********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) override;
    bool isDefaultConfiguration() const override;
  };
}
