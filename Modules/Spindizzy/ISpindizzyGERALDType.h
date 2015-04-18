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

#include "ISpindizzyGERALDSet.h"

class ElementSpindizzyCraft;

class ISpindizzyGERALDType:public IElementType {
  public:
  virtual ISpindizzyGERALDSet* getSpindizzyCraftInterface() = 0;
  virtual I3DModel* createModel(Vertex*) = 0;
  virtual void notifyMovement(ElementSpindizzyCraft*, Vertex&, Vertex&) = 0;
  virtual void notifyAppearance(ElementSpindizzyCraft*, Vertex&) = 0;
  virtual void notifyDisappearance(ElementSpindizzyCraft*, Vertex&) = 0;
  virtual ICollisionData* getNextEvent(Vertex&, Vertex&, IRollableSurface*) = 0;
  virtual IRollableSurface* getSurfaceAt(Vertex&) = 0;
  virtual ICamera* getCamera() = 0;
  virtual void executeRespawnScript() = 0;
  virtual void executeFallImpactScript(float, ElementSpindizzyCraft*) = 0;
  virtual std::string getInstanceName(ElementSpindizzyCraft*) = 0;
};

#endif
