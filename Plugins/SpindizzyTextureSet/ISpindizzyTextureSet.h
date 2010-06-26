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
#ifndef I_SPINDIZZY_TEXTURE_SET_H
#define I_SPINDIZZY_TEXTURE_SET_H

#include <IsoRealms/IPlugin.h>

#include "ISpindizzyTexture.h"

class ISpindizzyTextureSet:public virtual IPlugin {
  public:
  enum TextureType {
    SWITCH_CIRCLE_BOTH,
    SWITCH_CIRCLE_LEFT,
    SWITCH_CIRCLE_RIGHT,
    SWITCH_CIRCLE_NONE,
    SWITCH_SQUARE_BOTH,
    SWITCH_SQUARE_LEFT,
    SWITCH_SQUARE_RIGHT,
    SWITCH_SQUARE_NONE,
    SWITCH_DIAMOND_BOTH,
    SWITCH_DIAMOND_LEFT,
    SWITCH_DIAMOND_RIGHT,
    SWITCH_DIAMOND_NONE,
    SWITCH_RESET,
    LIFT_CIRCLE_BOTH,
    LIFT_CIRCLE_LEFT,
    LIFT_CIRCLE_RIGHT,
    LIFT_CIRCLE_NONE,
    LIFT_SQUARE_BOTH,
    LIFT_SQUARE_LEFT,
    LIFT_SQUARE_RIGHT,
    LIFT_SQUARE_NONE,
    LIFT_DIAMOND_BOTH,
    LIFT_DIAMOND_LEFT,
    LIFT_DIAMOND_RIGHT,
    LIFT_DIAMOND_NONE,
    ARROW_NORTH,
    ARROW_SOUTH,
    ARROW_EAST,
    ARROW_WEST,
    TRAMPOLINE,
    WATER,
    ICE,
    PLAIN,
    PLAIN_SPLIT_NE_SW,
    PLAIN_SPLIT_NW_SE,
    WALL_NORTH,
    WALL_EAST,
    WALL_SOUTH,
    WALL_WEST,
    WALL_ICE,
    BACKGROUND
  };

  /**
   * Return the texture of the specified type.
   * 
   * @param TextureType  The texture type.
   * @returns  The texture.
   */
  virtual ISpindizzyTexture* getTexture(TextureType) = 0;

  virtual ~ISpindizzyTextureSet() {}
};

#endif
