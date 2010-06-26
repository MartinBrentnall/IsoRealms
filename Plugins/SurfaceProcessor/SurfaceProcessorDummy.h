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
#ifndef SURFACE_PROCESSOR_DUMMY_H
#define SURFACE_PROCESSOR_DUMMY_H

#include <vector>

#include <IsoRealms/DummyPlugin.h>

#include "ISurfaceProcessor.h"

/**
 * This dummy implementation simply returns the surfaces provided by provider,
 * with no processing.
 */
class SurfaceProcessorDummy:public DummyPlugin,
                            public ISurfaceProcessor {
  private:
  int getOuterWallFaceLocation(BlockArea*, IWallSurface::FaceDirection);

  public:

  /********************************\
   * Implements ISurfaceProcessor *
  \********************************/
  void registerSurfaceProvider(ISurfaceProvider*);
  void unregisterSurfaceProvider(ISurfaceProvider*);
  std::vector<ITileSurfaceTemplate*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection);
  std::vector<IWallSurface*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection);
  void setDirty();
  void reinitialise();
};

#endif
