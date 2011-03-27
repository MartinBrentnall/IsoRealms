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
#ifndef I_SPINDIZZY_BLOCK_SET_H
#define I_SPINDIZZY_BLOCK_SET_H

#include <vector>

#include <IsoRealms/ElementSet.h>
#include <IsoRealms/Zone.h>

#include "../../Plugins/CollidableSurfaceRegistry/IRollableSurface.h"
#include "../../Plugins/CollidableSurfaceRegistry/ICollidableWallSurface.h"
#include "../../Plugins/SurfaceProcessor/ITileSurface.h"
#include "../../Plugins/SurfaceProcessor/ITileSurfaceTemplate.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProvider.h"
#include "../../Plugins/SurfaceProcessor/IWallSurface.h"

class ISpindizzyBlock;

class ISpindizzyBlockSet:public ElementSet<ISpindizzyBlock> {
  public:
  virtual void registerSurfaceProvider(ISurfaceProvider*) = 0;
  virtual void unregisterSurfaceProvider(ISurfaceProvider*) = 0;
  virtual void setDirty() = 0;
  virtual std::vector<ITileSurfaceTemplate*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection) = 0;
  virtual std::vector<IWallSurfaceTemplate*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection) = 0;
  virtual void notifyZoneAction(Zone*) = 0;
  virtual void registerRollableSurface(IRollableSurface*) = 0;
  virtual void registerWallSurface(ICollidableWallSurface*) = 0;
  virtual std::vector<ConditionElement*> getConditionElements() = 0;
  virtual void updateClue() = 0;
};

#endif
