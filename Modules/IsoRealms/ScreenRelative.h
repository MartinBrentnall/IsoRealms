/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef SCREEN_RELATIVE_H
#define SCREEN_RELATIVE_H

#include <IsoRealms/Configuration.h>

#include "IElementRelationManager.h"
#include "IHUDComponentRelation.h"

class ScreenRelative:public IHUDComponentRelation {
  private:
  enum ScreenEdge {
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
  };
  ScreenEdge cEdge;
  
  ScreenRelative(ScreenEdge);
  
  public:
  static ScreenRelative LEFT_EDGE;
  static ScreenRelative RIGHT_EDGE;
  static ScreenRelative BOTTOM_EDGE;
  static ScreenRelative TOP_EDGE;
    
  /************************************\
   * Implements IHUDComponentRelation *
  \************************************/
  float getLocation();
  void save(DOMNodeWriter*, const std::string&, IElementRelationManager*);
  void renderRelation();
};

#endif

