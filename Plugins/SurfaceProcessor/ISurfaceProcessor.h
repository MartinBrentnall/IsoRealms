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
#ifndef I_SURFACE_PROCESSOR_H
#define I_SURFACE_PROCESSOR_H

#include <vector>

#include "../../Global/IPlugin.h"

#include "ISurfaceProvider.h"
#include "ITileSurface.h"
#include "ITileSurfaceTemplate.h"
#include "IWallSurface.h"

class ISurfaceProcessor:public virtual IPlugin {
  public:

  virtual ~ISurfaceProcessor() {}

  /**
   * All elements using the surface processor must be registered by calling
   * this function during the first pass of the IElement::initElement() element
   * initialisation function.
   * 
   * New elements should call this during the IElement::added() function.
   * 
   * Calling this function from outside of these contexts will result in
   * undefined behaviour and possible crashes.
   * 
   * @param IRollableSurfaceProvider*  The provider to register.
   */
  virtual void registerSurfaceProvider(ISurfaceProvider*) = 0;

  /**
   * Unregister a surface provider with this surface calculation.  This
   * function should be called during the IElement::removed() function.
   * 
   * @param IRollableSurfaceProvider*  The provider to unregister.
   */
  virtual void unregisterSurfaceProvider(ISurfaceProvider*) = 0;

  /**
   * Notify the surface processor that cached providers may have changed.
   */
  virtual void setDirty() = 0;

  /**
   * Cause the surface processor to reinitialise.  This method should be called
   * when an element set is removed that was using the surface processor.
   */
  virtual void reinitialise() = 0;

  /**
   * Retrieve the surfaces from the specified provider according to the rules
   * of this surface calculator.
   * 
   * @param IRollableSurfaceProvider*  The provider to calculate surfaces for.
   * @returns  A vector containing the calculated surfaces.
   */
  virtual std::vector<ITileSurfaceTemplate*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection) = 0;

  /**
   * Retrieve the wall surfaces from the specified provider according to the
   * rules of this surface calculator.
   * 
   * @param IRollableSurfaceProvider*  The provider to calculate surfaces for.
   * @param IWallSurface::FaceDirection  The facing direction of walls to return.
   * @returns  A vector containing the calculated wall surfaces.
   */
  virtual std::vector<IWallSurface*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection) = 0;
};

#endif
