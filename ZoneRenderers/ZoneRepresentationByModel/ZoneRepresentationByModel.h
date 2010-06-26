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
#ifndef ZONE_REPRESENTATION_BY_MODEL_H
#define ZONE_REPRESENTATION_BY_MODEL_H

#include <IsoRealms/MapRenderer.h>

/**
 * This Map Renderer uses a 3DModelFactory plugin instance to create a model
 * that represents each zone.  Each model instance is scaled and positioned to
 * appropriately represent the size and location of each zone.
 */
class ZoneRepresentationByModel:public MapRenderer {
  public:
  ZoneRepresentationByModel(IMap*);
  
  /**************************\
   * Implements MapRenderer *
  \**************************/
  void render();
  void update(unsigned int ticks);
};

#endif
