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
#ifndef I_BOUNDARIES_H
#define I_BOUNDARIES_H

#include <IsoRealms/Resources/IResourceManageable.h>
#include <IsoRealms/Resources/Script/IArgumentValueCollection.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "IBoundary.h"
#include "IBoundaryPenetrator.h"

class BoundaryHandler;

class IBoundaries:public IResourceManageable<IBoundaries> {
  public:
  virtual void registerArgumentValuesBoundaries(IArgumentValueCollection*) = 0;
  virtual void registerArgumentValuesBoundaryPenetrator(IArgumentValueCollection*) = 0;
  virtual void registerBoundary(IBoundary*) = 0;
  virtual void notifyMovement(IBoundaryPenetrator*, Vertex&, Vertex&, BoundaryHandler*) = 0;
  virtual void notifyAppearance(IBoundaryPenetrator*, Vertex&, BoundaryHandler*) = 0;
  virtual void notifyDisappearance(IBoundaryPenetrator*, Vertex&, BoundaryHandler*) = 0;
  virtual void reinitialise() = 0;
};

#endif
