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

#include <IsoRealms/Resources/ElementType/ElementSet.h>
#include <IsoRealms/Resources/SurfaceProcessor/ITileSurface.h>
#include <IsoRealms/Resources/SurfaceProcessor/ITileSurfaceTemplate.h>
#include <IsoRealms/Resources/SurfaceProcessor/ISurfaceProcessor.h>
#include <IsoRealms/Resources/SurfaceProcessor/ISurfaceProvider.h>
#include <IsoRealms/Resources/SurfaceProcessor/IWallSurface.h>
#include <IsoRealms/Resources/SurfaceRegistry/ICollidableWallSurface.h>
#include <IsoRealms/Resources/SurfaceRegistry/IRollableSurface.h>
#include <IsoRealms/Zone.h>

class ISpindizzyBlock;

class ISpindizzyBlockSet:public ElementSet<ISpindizzyBlock> {
  public:
  virtual void registerSurfaceProvider(ISurfaceProvider*) = 0;
  virtual void unregisterSurfaceProvider(ISurfaceProvider*) = 0;
  
  /**
   * The intention of this function is not simply to set all elements of this
   * element set to dirty, but to call upon the surface processors to set all
   * elements within _them_ to dirty.  This is necessary because changes in an
   * element of this set may affect the surfaces of elements from another set.
   */
  virtual void setDirty() = 0;
  
  virtual std::vector<ITileSurfaceTemplate*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection, bool) = 0;
  virtual std::vector<IWallSurfaceTemplate*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection, bool) = 0;
  virtual void destroyTileTemplate(ITileSurfaceTemplate*, bool) = 0;
  virtual void destroyWallTemplate(IWallSurfaceTemplate*, bool) = 0;
  virtual void registerRollableSurface(IRollableSurface*) = 0;
  virtual void registerWallSurface(ICollidableWallSurface*) = 0;
  virtual std::vector<ConditionElement*> getConditionElements() = 0;
  virtual void updateClue() = 0;
  virtual bool isEditing() = 0;
  virtual void staticChanged() = 0;
};

#endif
