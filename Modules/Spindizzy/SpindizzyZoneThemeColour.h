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
#ifndef SPINDIZZY_ZONE_THEME_COLOUR_H
#define SPINDIZZY_ZONE_THEME_COLOUR_H

#include <IsoRealms/Resources/Colour/IColour.h>

class SpindizzyZoneThemeColour:public IColour {
  private:
  IColour* cColour;

  public:
  void set(IColour*);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  /**********************\
   * Implements IColour *
  \**********************/
  float getRed() const;
  float getGreen() const;
  float getBlue() const;
  float getAlpha() const;
  int getIntRed() const;
  int getIntGreen() const;
  int getIntBlue() const;
  int getIntAlpha() const;
  float luminance();
  void set();
  void setBackground();
};

#endif
