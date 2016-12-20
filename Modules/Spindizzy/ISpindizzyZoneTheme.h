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
#ifndef I_SPINDIZZY_ZONE_THEME_H
#define I_SPINDIZZY_ZONE_THEME_H

#include <map>

#include <IsoRealms/GUI/Icon.h>
#include <IsoRealms/Resources/Colour/IColour.h>

#include "SpindizzyZoneThemeColour.h"
#include "SpindizzyZoneThemeTexture.h"

class ISpindizzyZoneTheme:public IResource {
  public:
  virtual Icon<ISpindizzyZoneTheme>* createCustomResourceIcon(IResourceBrowser<ISpindizzyZoneTheme>*) = 0;
  virtual void set() = 0;
  virtual IColour* getColour(SpindizzyZoneThemeColour*) = 0;
  
  // Editing functions.  TODO: Separate into different interface?
  virtual std::string getName() = 0;
  virtual std::map<SpindizzyZoneThemeTexture*, ITexture*> getTextureElements() = 0;
  virtual std::map<SpindizzyZoneThemeColour*, IColour*> getColourElements() = 0;
  virtual std::string getElementName(SpindizzyZoneThemeTexture*) = 0;
  virtual std::string getElementName(SpindizzyZoneThemeColour*) = 0;
  virtual void registerElement(SpindizzyZoneThemeTexture*, ITexture*) = 0;
  virtual void registerElement(SpindizzyZoneThemeColour*, IColour*) = 0;
  virtual void hintInUse(bool) = 0;
};

#endif

