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
#ifndef I_ZONE_RENDERER_H
#define I_ZONE_RENDERER_H

#include <vector>

#include "IProject.h"
#include "IZone.h"

class IZoneRenderer {
  public:
    
  /**
   * TODO: Interface should NOT expose whole zone!
   */
  virtual void render(std::vector<IZone*>&, IProject*) = 0;
  virtual void update(std::vector<IZone*>&, unsigned int) = 0;
  virtual void updateRuntime(std::vector<IZone*>&, unsigned int) = 0;
};

#endif
