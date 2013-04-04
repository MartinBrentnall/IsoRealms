/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef I_SPINDIZZY_GERALD_TYPE_H
#define I_SPINDIZZY_GERALD_TYPE_H

#include <IsoRealms/Resources/ElementType/ElementType.h>

#include "ISpindizzyGERALDSet.h"

class SpindizzyGERALD;

class ISpindizzyGERALDType:public ElementType<ISpindizzyGERALDSet, SpindizzyGERALD> {
  public:
  ISpindizzyGERALDType(ISpindizzyGERALDSet* elementSet) : ElementType<ISpindizzyGERALDSet, SpindizzyGERALD>(elementSet) {}
  
  virtual I3DModel* createModel(Vertex*) = 0;
  virtual void collect(SpindizzyGERALD*, Vertex&, Vertex&) = 0;
  virtual ICollisionData* getNextEvent(Vertex&, Vertex&, IRollableSurface*) = 0;
  virtual IRollableSurface* getSurfaceAt(Vertex&) = 0;
  virtual ICamera* getCamera() = 0;
  virtual void executeRespawnScript() = 0;
  virtual void executeFallImpactScript(float, SpindizzyGERALD*) = 0;
  virtual void executeZoneEnteredScript(IZone*) = 0;
  virtual void executeZoneExitedScript(IZone*) = 0;
  virtual std::string getInstanceName(SpindizzyGERALD*) = 0;
};

#endif
