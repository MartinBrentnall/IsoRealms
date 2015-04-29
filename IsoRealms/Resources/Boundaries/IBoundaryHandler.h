/*
 * Copyright 2015 Martin Brentnall
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
#ifndef I_BOUNDARY_HANDLER_H
#define I_BOUNDARY_HANDLER_H

#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/Script/IArgumentValueCollection.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "IBoundary.h"
#include "IBoundaryPenetrator.h"

class IBoundaryHandler {
  public:
  virtual void notifyMovement(IBoundaryPenetrator*, Vertex&, Vertex&) = 0;
  virtual void notifyAppearance(IBoundaryPenetrator*, Vertex&) = 0;
  virtual void notifyDisappearance(IBoundaryPenetrator*, Vertex&) = 0;
};

#endif
