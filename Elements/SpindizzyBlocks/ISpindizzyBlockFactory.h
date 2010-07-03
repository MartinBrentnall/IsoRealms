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
#ifndef I_SPINDIZZY_BLOCK_FACTORY_H
#define I_SPINDIZZY_BLOCK_FACTORY_H

#include <IsoRealms/ElementFactory.h>

#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"

#include "ISpindizzyBlockSet.h"

/**
 * TODO: Not an interface anymore.
 */
class ISpindizzyBlockFactory:public ElementFactory<ISpindizzyBlockSet, IElement> {
  public:
  ISpindizzyBlockFactory(ISpindizzyBlockSet*);

  /**
   * Signal all elements of the factory as dirty.
   */
  virtual void signalAllElementsDirty() = 0;

  /**
   * Unregister all the blocks (surface providers) with the specified surface
   * calculator.
   * 
   * @param ISurfaceProcessor  The surface calculator to unregister on.
   */
  virtual void unregisterSurfaces(ISurfaceProcessor*) = 0;
};

#endif
