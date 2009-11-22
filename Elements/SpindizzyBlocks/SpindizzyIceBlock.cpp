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
#include "SpindizzyIceBlock.h"

SpindizzyIceBlock::SpindizzyIceBlock(IElementFactory* elementFactory, BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** spindizzyTextureSet, SpindizzyBlockProperties* blockProperties, bool addition) : AbstractSpindizzyBlock(elementFactory, startLocation, endLocation, spindizzyTextureSet, blockProperties, addition) {
}

ISpindizzyTextureSet::TextureType SpindizzyIceBlock::getRollableSurfaceTexture() {
  return ISpindizzyTextureSet::ICE;
}

ISpindizzyTextureSet::TextureType SpindizzyIceBlock::getNorthWallTexture() {
  // TODO: NORTH_ICE, etc.
  return ISpindizzyTextureSet::WALL_ICE;
}

ISpindizzyTextureSet::TextureType SpindizzyIceBlock::getEastWallTexture() {
  return ISpindizzyTextureSet::WALL_ICE;
}

ISpindizzyTextureSet::TextureType SpindizzyIceBlock::getSouthWallTexture() {
  return ISpindizzyTextureSet::WALL_ICE;
}

ISpindizzyTextureSet::TextureType SpindizzyIceBlock::getWestWallTexture() {
  return ISpindizzyTextureSet::WALL_ICE;
}
