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
#include "SpindizzyTrampolineBlock.h"

SpindizzyTrampolineBlock::SpindizzyTrampolineBlock(IElementFactory* elementFactory, BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** spindizzyTextureSet, SpindizzyBlockProperties* blockProperties, bool addition) : AbstractSpindizzyBlock(elementFactory, startLocation, endLocation, spindizzyTextureSet, blockProperties, addition) {
}

ISpindizzyTextureSet::TextureType SpindizzyTrampolineBlock::getRollableSurfaceTexture() {
  return ISpindizzyTextureSet::TRAMPOLINE;
}

ISpindizzyTextureSet::TextureType SpindizzyTrampolineBlock::getNorthWallTexture() {
  return ISpindizzyTextureSet::WALL_NORTH;
}

ISpindizzyTextureSet::TextureType SpindizzyTrampolineBlock::getEastWallTexture() {
  return ISpindizzyTextureSet::WALL_EAST;
}

ISpindizzyTextureSet::TextureType SpindizzyTrampolineBlock::getSouthWallTexture() {
  return ISpindizzyTextureSet::WALL_SOUTH;
}

ISpindizzyTextureSet::TextureType SpindizzyTrampolineBlock::getWestWallTexture() {
  return ISpindizzyTextureSet::WALL_WEST;
}
