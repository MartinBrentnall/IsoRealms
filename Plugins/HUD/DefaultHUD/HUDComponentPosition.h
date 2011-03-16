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
#include <IsoRealms/ScreenConfiguration.h>

#include "../IHUD.h"

class HUDComponentPosition {
  private:
  IHUDGameComponent* cComponent;
  float cScale;
  float cXPosition;
  float cYPosition;
  float cXAlign;
  float cYAlign;

  public:
  HUDComponentPosition(IHUDGameComponent*);
  void setScale(float);
  void setXPosition(float);
  void setYPosition(float);
  void setXAlign(float);
  void setYAlign(float);
  void update(int);
  void render();
};

#endif
