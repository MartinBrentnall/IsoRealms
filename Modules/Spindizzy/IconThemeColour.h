/*
 * Copyright 2016 Martin Brentnall
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
#ifndef ICON_THEME_COLOUR_H
#define ICON_THEME_COLOUR_H

#include <IsoRealms/GUI/Icon.h>
#include <IsoRealms/Resources/Colour/Colour.h>

#include "IIconAnimator.h"
#include "SpindizzyZoneThemeColour.h"

class IconThemeColour:public Icon<IColour> {
  private:
  IIconAnimator* cResourceAnimator;
  SpindizzyZoneThemeColour* cColour;
  
  public:
  IconThemeColour(IIconAnimator*, IResourceBrowser<IColour>*, SpindizzyZoneThemeColour*);

  void renderIcon();  
  float getWidth();
  float getHeight();
};

#endif
