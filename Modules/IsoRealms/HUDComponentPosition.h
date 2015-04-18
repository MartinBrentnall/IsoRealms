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
#ifndef HUD_COMPONENT_POSITION_H
#define HUD_COMPONENT_POSITION_H

#include <map>
#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/HUDComponents/IHUDGameComponent.h>
#include <IsoRealms/ScreenConfiguration.h>

#include "IComponentSources.h"
#include "IHUDComponentRelation.h"

class HUDComponentPosition {
  private:
  IHUDGameComponent* cComponent;
  float cXScale;
  float cYScale;
  IHUDComponentRelation* cLeftRelation;
  IHUDComponentRelation* cRightRelation;
  IHUDComponentRelation* cTopRelation;
  IHUDComponentRelation* cBottomRelation;

  float getXScale();
  float getYScale();
  float getXPosition();
  float getYPosition();  
  
  public:
  HUDComponentPosition(IHUDGameComponent*, IHUDComponentRelation*, IHUDComponentRelation*, IHUDComponentRelation*, IHUDComponentRelation*, float, float);
  void update(unsigned int);
  void render();
  float getLeft();
  float getRight();
  float getBottom();
  float getTop();
  void save(DOMNodeWriter*, IComponentSources*);
};

#endif
