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

#include <string>

namespace IsoRealms {
  class JSONObject;

  class PropertyData {
    public:
    PropertyData(const std::string& name, const std::string& tooltip);
    PropertyData(JSONObject object);
    PropertyData(const PropertyData& other);

    std::string getName() const;
    std::string getTooltip() const;

    private:
    inline static const std::string JSON_DESCRIPTION = "description";
    inline static const std::string JSON_NAME        = "name";

    const std::string cName;
    const std::string cTooltip;
  };
}
