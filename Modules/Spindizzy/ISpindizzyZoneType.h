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
#ifndef I_SPINDIZZY_ZONE_TYPE_H
#define I_SPINDIZZY_ZONE_TYPE_H

#include <IsoRealms/Resources/ElementType/IElementType.h>

#include "ISpindizzyZoneModule.h"

class ElementSpindizzyZone;

class ISpindizzyZoneType:public IElementType {
  public:
  virtual void zoneGainedFocus(ElementSpindizzyZone*) = 0;
  virtual void zoneLostFocus(ElementSpindizzyZone*) = 0;
  virtual ISpindizzyZoneModule* getSpindizzyZoneInterface() = 0;
  virtual bool isOverview() = 0;
  virtual IColour* getVisitedColour() = 0;
  virtual IColour* getUnvisitedColour() = 0;
  virtual IBoundaries* getBoundaries() = 0;
  virtual void setZoneArgument(ElementSpindizzyZone*) = 0;
  virtual void applyDefaultTheme() = 0;
  virtual void setDefaultTheme(SpindizzyZoneTheme*) = 0;
};

#endif
