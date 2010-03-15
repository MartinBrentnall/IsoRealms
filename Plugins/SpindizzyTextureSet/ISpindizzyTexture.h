/*
 * Copyright 2009 Martin Brentnall
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
#ifndef I_SPINDIZZY_TEXTURE_H
#define I_SPINDIZZY_TEXTURE_H

#include "../../Global/Colour.h"

class ISpindizzyTexture {
  public:
  enum Mapping {

    /**
     * The texture is to be tiled, one instance per block.
     */
    TILED,

    /**
     * Used for wall columns.  The top quarter of the texture is used for the
     * top half of the top block.  Likewise, the bottom quarter of the texture
     * is to be used for the bottom half of the bottom block.  The remaining
     * central half of the texture is to be used in a repeating pattern down
     * the remaining section of the column.
     * 
     * The texture should be design so that it looks as though it shows exactly
     * two blocks.  However, it should be noted that the texture may be used
     * without the center half (i.e. one block high), or with multiple
     * instances of the center half (i.e. more than two blocks high), and even
     * with sloped top and bottom sections.
     */
    COLUMN_CAPPED,

    PLAIN_COLOUR
  };

  virtual void set() = 0;
  virtual void texCoord2f(float, float) = 0;
  virtual Colour* getColour(float, float) = 0;

  /**
   * Provide a hint regarding the intended use of this texture.
   * 
   * @returns A hint regarding the way this texture is to be mapped.
   */
  virtual Mapping getMapping() = 0;

  virtual ~ISpindizzyTexture() {}
};

#endif
