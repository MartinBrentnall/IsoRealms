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
#ifndef I_SURFACE_PROCESSOR_PROXY_H
#define I_SURFACE_PROCESSOR_PROXY_H

#include <vector>

#include "../../Global/Zone.h"

#include "../../Plugins/SurfaceProcessor/ITileSurface.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProvider.h"
#include "../../Plugins/SurfaceProcessor/IWallSurface.h"

// TODO: Rename
class ISurfaceProcessorProxy:public IElementSet {
  public:
  virtual void registerSurfaceProvider(ISurfaceProvider*) = 0;
  virtual void unregisterSurfaceProvider(ISurfaceProvider*) = 0;
  virtual void setDirty() = 0;
  virtual std::vector<ITileSurface*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection) = 0;
  virtual std::vector<IWallSurface*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection) = 0;
  virtual void notifyZoneAction(Zone*) = 0;
};

#endif
