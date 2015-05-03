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
#ifndef I_BOUNDARY_H
#define I_BOUNDARY_H

#include <IsoRealms/Resources/ElementType/IElementBounds.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class IBoundary {
  public:
  virtual bool contains(Vertex&) = 0;
  virtual bool isEntered(Vertex&, Vertex&) = 0;
  virtual bool isExited(Vertex&, Vertex&) = 0;
  virtual void setDirty() = 0;
  virtual IElementBounds* getBounds() = 0;
  virtual void setArguments() = 0;
  virtual void unsetArguments() = 0;
};

#endif
