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

#include <IsoRealms/IConditionElementIcons.h>

#include "ICollidableSurfaceElement.h"
#include "ICollidableWallSurface.h"
#include "IRollableSurface.h"
#include "ITileSurface.h"
#include "ITileSurfaceTemplate.h"
#include "IGeometricElement.h"
#include "IWallSurface.h"
#include "IWallSurfaceTemplate.h"

class ElementHandlerSpindizzyBlock;
class ISpindizzyBlock;
class SpindizzyBlockState;

class ISpindizzyBlockSet:public IConditionElementIcons {
  public:
  virtual void registerSurfaceProvider(IGeometricElement*, bool, IUniverse*) = 0;
  virtual void unregisterSurfaceProvider(IGeometricElement*) = 0;
  
  /**
   * The intention of this function is not simply to set all elements of this
   * element set to dirty, but to call upon the surface processors to set all
   * elements within _them_ to dirty.  This is necessary because changes in an
   * element of this set may affect the surfaces of elements from another set.
   */
  virtual void setDirty() = 0;
  
  virtual std::vector<ITileSurfaceTemplate*> getTileSurfaces(IGeometricElement*, ITileSurface::FaceDirection, bool) = 0;
  virtual std::vector<IWallSurfaceTemplate*> getWallSurfaces(IGeometricElement*, IWallSurface::FaceDirection, bool) = 0;
  virtual void destroyTileTemplate(IGeometricElement*, ITileSurfaceTemplate*, bool) = 0;
  virtual void destroyWallTemplate(IGeometricElement*, IWallSurfaceTemplate*, bool) = 0;
  virtual void registerRollableSurface(ICollidableSurfaceElement*, IRollableSurface*, IUniverse*) = 0;
  virtual void registerWallSurface(ICollidableSurfaceElement*, ICollidableWallSurface*, IUniverse*) = 0;
  virtual void unregisterSurfaces(ICollidableSurfaceElement*) = 0;
  virtual void unregisterRollableSurface(IRollableSurface*) = 0;
  virtual void unregisterWallSurface(ICollidableWallSurface*) = 0;
  virtual void saveCachePhysicalSurfaces(DOMNodeWriter*, ICollidableSurfaceElement*) = 0;
  virtual std::vector<ConditionElement*> getConditionElements() = 0;
  virtual bool isEditing() = 0;
  virtual void staticChanged() = 0;
  
  virtual SpindizzyBlockState* getBlockState(IBoolean*) = 0;
  virtual ElementHandlerSpindizzyBlock* getElementHandlerSpindizzyBlock(IElementContainer*) = 0;
  virtual void removeElementHandlerSpindizzyBlock(IElementContainer*) = 0;
  virtual void setArgumentValue(ElementHandlerSpindizzyBlock*) = 0;
  virtual bool isUsingCache() = 0;  
};

#endif
