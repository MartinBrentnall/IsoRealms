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
#ifndef I_ROLLABLE_SURFACE_CALCULATOR_H
#define I_ROLLABLE_SURFACE_CALCULATOR_H

#include <vector>

#include "../../Global/IPlugin.h"

#include "IRollableSurface.h"
#include "IRollableSurfaceProvider.h"
#include "IWallSurface.h"

class IRollableSurfaceCalculator:public virtual IPlugin {
  public:

  virtual ~IRollableSurfaceCalculator() {}

  /**
   * This method offers the surface calculator an opportunity to obtain surface
   * providers that it will be working with during initialization of elements.
   * 
   * @param IRollableSurfaceProvider*  The provider to register.
   */
  virtual void registerRollableSurfaceProvider(IRollableSurfaceProvider*) = 0;

  /**
   * Unregister a surface provider with this surface calculation.
   * 
   * @param IRollableSurfaceProvider*  The provider to unregister.
   */
  virtual void unregisterRollableSurfaceProvider(IRollableSurfaceProvider*) = 0;

  /**
   * Notify the surface calculator that cached providers may have changed.
   */
  virtual void setDirty() = 0;

  /**
   * Retrieve the surfaces from the specified provider according to the rules
   * of this surface calculator.
   * 
   * @param IRollableSurfaceProvider*  The provider to calculate surfaces for.
   * @returns  A vector containing the calculated surfaces.
   */
  virtual std::vector<IRollableSurface*> getRollableSurfaces(IRollableSurfaceProvider*, IRollableSurface::FaceDirection) = 0;

  /**
   * Retrieve the wall surfaces from the specified provider according to the
   * rules of this surface calculator.
   * 
   * @param IRollableSurfaceProvider*  The provider to calculate surfaces for.
   * @param IWallSurface::FaceDirection  The facing direction of walls to return.
   * @returns  A vector containing the calculated wall surfaces.
   */
  virtual std::vector<IWallSurface*> getWallSurfaces(IRollableSurfaceProvider*, IWallSurface::FaceDirection) = 0;
};

#endif
