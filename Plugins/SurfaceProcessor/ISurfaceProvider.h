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
#ifndef I_SURFACE_PROVIDER_H
#define I_SURFACE_PROVIDER_H

#include <vector>

#include "ITileSurface.h"
#include "IWallSurface.h"

#include <IsoRealms/BlockArea.h>

/**
 * This class is to be implemented by objects being provided to the processor.
 */
class ISurfaceProvider {
  public:

  /**
   * This can be used by algorithms to perform optimizations.
   * 
   * @returns TODO
   */
  virtual BlockArea* getCoverage() = 0;

  /**
   * Get all of the tile surfaces from this block.  No two surfaces should be
   * alligned on the Z-axis, and should not exceed the area defined by the
   * value of getCoverage(); failure to adhere to these rule will result in
   * undefined behaviour.  
   * 
   * @param IRollableSurface::FaceDirection  The face direction of the surface
   *           to get.
   * @returns  The tile surfaces of this object.
   */
  virtual std::vector<ITileSurface*> getTileSurfaces(ITileSurface::FaceDirection) = 0;
 
  /**
   * Get a raw unprocessed wall surface of this object.
   * 
   * @param int  The X / Y of the wall to get (depending on facing direction).
   * @param FaceDirection  The facing direction of the wall to get.
   * @return  The unprocessed wall facing the specified direction at the
   *          specified location.
   */
  virtual std::vector<IWallSurface*> getWallSurfaces(int, IWallSurface::FaceDirection) = 0;

  virtual IWallSurface* createSubSurface(int, int, IWallSurface::FaceDirection, int, int, int, int, int) = 0;

  virtual bool isGhost() = 0;
  
  /**
   * Signal to the element of the provider that it needs reinitialization.
   */
  virtual void setDirty() = 0;
};

#endif
