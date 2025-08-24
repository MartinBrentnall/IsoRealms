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

#include "IsoRealms/Assets/Type/IString.h"

namespace IsoRealms {
  class Metadata;
  class Project;
  
  /**
   * A simple fixed String asset implementation.
   */
  class LiteralString : public IString {
    public:

    /**
     * Construct the String with the specified value.
     *
     * @param value Value of the String.
     */
    LiteralString(const std::string& value = "");

    void setValue(const std::string& value);

    /**********************\
     * Implements IString *
    \**********************/
    std::string getValue() const override;

    /**********************************\
     * Implements IAsset from IString *
    \**********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    static const std::string JSON_VALUE;

    std::string cValue; /// The value of this String.
  };
}
