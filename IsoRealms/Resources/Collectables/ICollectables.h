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
#ifndef I_COLLECTABLES_H
#define I_COLLECTABLES_H

#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "ICollectable.h"
#include "ICollector.h"

class ICollectables:public IResource {
  public:
  virtual void registerCollectable(ICollectable*) = 0;
  virtual void collect(ICollector*, Vertex&, Vertex&) = 0;
  virtual void reinitialise() = 0;
};

#endif
