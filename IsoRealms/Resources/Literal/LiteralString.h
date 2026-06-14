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

#include "IsoRealms/Resources/Type/IString.h"

namespace IsoRealms {
  class Metadata;
  class Project;
  
  /**
   * A simple fixed String resource implementation.
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
    void saveResource(JSONObject object) const override;
    bool isDefaultConfiguration() const override;
    std::string getValue() const override;

    /*************************************\
     * Implements IResource from IString *
    \*************************************/
    private:
    inline static const std::string JSON_VALUE = "value";

    std::string cValue; /// The value of this String.
  };
}
