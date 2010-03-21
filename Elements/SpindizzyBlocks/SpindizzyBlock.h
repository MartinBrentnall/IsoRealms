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
#ifndef SPINDIZZY_BLOCK_H
#define SPINDIZZY_BLOCK_H

#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"

#include "AbstractSpindizzyBlock.h"
#include "ISpindizzyBlockFactory.h"

class SpindizzyBlock:public AbstractSpindizzyBlock {
  private:
  ISpindizzyTextureSet::TextureType cTileSurfaceTexture;

  public:
  SpindizzyBlock(ISpindizzyBlockFactory*, BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType, SpindizzyBlockProperties*, bool);

  /*************************************\
   * Implements AbstractSpindizzyBlock *
  \*************************************/
  ISpindizzyTextureSet::TextureType getTileSurfaceTexture();
  ISpindizzyTextureSet::TextureType getNorthWallTexture();
  ISpindizzyTextureSet::TextureType getEastWallTexture();
  ISpindizzyTextureSet::TextureType getSouthWallTexture();
  ISpindizzyTextureSet::TextureType getWestWallTexture();
};

#endif
