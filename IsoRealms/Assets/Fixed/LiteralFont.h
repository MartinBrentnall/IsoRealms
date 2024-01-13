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

#include "IsoRealms/Assets/Type/IFont.h"

namespace IsoRealms {
  class LiteralFont : public IFont {
    public:
    
    /********************\
     * Implements IFont *
    \********************/
    void print(float x, float y, float size, Alignment alignment, const std::string& text) override;
    float getWidth(float size, const std::string& text) override;
    float getHeight(float size, const std::string& text) override;
    unsigned int getChar(float position, float size, const std::string& text) override;
    bool renderAssetIcon() const override;
  };
}
