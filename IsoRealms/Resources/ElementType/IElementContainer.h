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
#ifndef I_ELEMENT_CONTAINER_H
#define I_ELEMENT_CONTAINER_H

#include <IsoRealms/BlockArea.h>
#include <IsoRealms/Resources/Script/IArgument.h>

#include "IElement.h"

class IElementContainer {
  public:
  virtual void addElement(IElement*) = 0;
  virtual void removeElement(IElement*) = 0;
  virtual void updateElement(IElement*) = 0;
  virtual void addArgumentValue(IArgument*) = 0;
  virtual void setArguments() = 0;
  virtual void unsetArguments() = 0;
  virtual BlockArea* getCoverage() = 0;
  virtual void setDirty(IElement*) = 0;
  virtual void restrictCursor(Vertex&) = 0;
};

#endif
