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
#ifndef ROLLABLE_SURFACE_CALCULATOR_DUMMY_H
#define ROLLABLE_SURFACE_CALCULATOR_DUMMY_H

#include <vector>

#include "../../Global/DummyPlugin.h"

#include "IRollableSurfaceCalculator.h"

/**
 * This dummy implementation simply returns the surfaces provided by provider,
 * with no processing.
 */
class RollableSurfaceCalculatorDummy:public DummyPlugin,
                                     public IRollableSurfaceCalculator {
  private:
  int getOuterWallFaceLocation(BlockArea*, IWallSurface::FaceDirection);

  public:

  /*****************************************\
   * Implements IRollableSurfaceCalculator *
  \*****************************************/
  void registerRollableSurfaceProvider(IRollableSurfaceProvider*);
  void unregisterRollableSurfaceProvider(IRollableSurfaceProvider*);
  std::vector<IRollableSurface*> getRollableSurfaces(IRollableSurfaceProvider*, IRollableSurface::FaceDirection);
  std::vector<IWallSurface*> getWallSurfaces(IRollableSurfaceProvider*, IWallSurface::FaceDirection);
  void setDirty();
};

#endif
