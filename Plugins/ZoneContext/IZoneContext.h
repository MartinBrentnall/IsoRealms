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
#ifndef I_ZONE_CONTEXT_H
#define I_ZONE_CONTEXT_H

#include "../../Global/IPlugin.h"
#include "../../Global/IZone.h"

#include "IZoneContextListener.h"

class IZoneContext:public virtual IPlugin {
  public:
  virtual void setZoneContext(IZone*) = 0;
  virtual IZone* getZoneContext() = 0;
  virtual void addZoneContextListener(IZoneContextListener*) = 0;
  virtual void removeZoneContextListener(IZoneContextListener*) = 0;
};

#endif
