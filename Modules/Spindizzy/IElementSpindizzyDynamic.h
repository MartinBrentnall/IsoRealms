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
#ifndef I_ELEMENT_SPINDIZZY_DYNAMIC_H
#define I_ELEMENT_SPINDIZZY_DYNAMIC_H

#include <IsoRealms/ILayerEditingContext.h>
#include <IsoRealms/Resources/ElementType/IElementBounds.h>

class IElementSpindizzyDynamic {
  public:
  virtual void reset() = 0;
  virtual void renderEditing() = 0;
  virtual void renderRuntime() = 0;
  virtual void updateRuntime(unsigned int) = 0;
  virtual bool initElement(unsigned int) = 0;  
  virtual IElementBounds* getBounds() = 0;
  virtual void processCursorMovement(ILayerEditingContext*, Vertex&, Vertex&) = 0;
  virtual void processCursorAppearance(ILayerEditingContext*, Vertex&) = 0;
};

#endif
