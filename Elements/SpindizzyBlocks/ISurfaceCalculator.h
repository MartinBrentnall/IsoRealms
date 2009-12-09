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
#ifndef I_SURFACE_CALCULATOR_H
#define I_SURFACE_CALCULATOR_H

#include <vector>

#include "../../Global/Zone.h"

#include "../../Plugins/RollableSurfaceCalculator/IRollableSurface.h"
#include "../../Plugins/RollableSurfaceCalculator/IRollableSurfaceCalculator.h"
#include "../../Plugins/RollableSurfaceCalculator/IRollableSurfaceProvider.h"
#include "../../Plugins/RollableSurfaceCalculator/IWallSurface.h"

class ISurfaceCalculator {
  public:
  virtual void registerRollableSurfaceProvider(IRollableSurfaceProvider*) = 0;
  virtual void unregisterRollableSurfaceProvider(IRollableSurfaceProvider*) = 0;
  virtual void setDirty() = 0;
  virtual std::vector<IRollableSurface*> getRollableSurfaces(IRollableSurfaceProvider*, IRollableSurface::FaceDirection) = 0;
  virtual std::vector<IWallSurface*> getWallSurfaces(IRollableSurfaceProvider*, IWallSurface::FaceDirection) = 0;
  virtual void notifyZoneAction(Zone*) = 0;
};

#endif
