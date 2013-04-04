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
#ifndef I_SPINDIZZY_BLOCK_TYPE_H
#define I_SPINDIZZY_BLOCK_TYPE_H

#include <IsoRealms/Resources/ElementType/ElementType.h>
#include <IsoRealms/Resources/SurfaceProcessor/ISurfaceProcessor.h>

#include "BlockTypeProperties.h"
#include "ISpindizzyBlockSet.h"

class ISpindizzyBlock;

class ISpindizzyBlockType:public ElementType<ISpindizzyBlockSet, ISpindizzyBlock> {
  public:
  ISpindizzyBlockType(ISpindizzyBlockSet*);

  /**
   * Unregister all the blocks (surface providers) with the specified surface
   * calculator.
   * 
   * @param ISurfaceProcessor  The surface calculator to unregister on.
   */
  virtual void unregisterSurfaces(ISurfaceProcessor*) = 0;
  
  virtual BlockTypeProperties* getBlockTypeProperties() = 0;
};

#endif
