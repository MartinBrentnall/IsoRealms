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

#include "Modules/UI/Layout/ILayoutOffset.h"

namespace IsoRealms::UI {
  
  /**
   * A layout offset with a fixed value.
   */
  class AbsoluteOffset : public ILayoutOffset {
    public:
    AbsoluteOffset(float value);
    AbsoluteOffset(JSONObject object);

    /****************************\
     * Implements ILayoutOffset *
    \****************************/
    float getOffset(float aspectRatio) const override;
    void save(JSONObject object, Layout* layout) const override;

    private:

    // JSON members.
    static const std::string JSON_TYPE;
    static const std::string JSON_VALUE;

    static const std::string TYPE_ABSOLUTE;

    // Definition data.
    float cDefValue; // The offset value.
  };
}
