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
#ifndef SPINDIZZY_ZONE_THEME_H
#define SPINDIZZY_ZONE_THEME_H

#include <map>

#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceUseListener.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "SpindizzyZoneThemeColour.h"
#include "SpindizzyZoneThemeIcon.h"
#include "SpindizzyZoneThemeTexture.h"
#include "ISpindizzyZoneTheme.h"
#include "IThemeSource.h"

class SpindizzyZoneTheme:public IResourceUseListener<ITexture>,
                         public IResourceUseListener<IColour>,
                         public ISpindizzyZoneTheme {
  private:
  IResourceAccessor* cResources;
  std::map<SpindizzyZoneThemeTexture*, ITexture*> cTextures;
  std::map<SpindizzyZoneThemeColour*, IColour*> cColours;
  IThemeSource* cThemeSource;

  public:
  SpindizzyZoneTheme(IThemeSource*);
    
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
    
  ITexture* getTexture(SpindizzyZoneThemeTexture*);
  void removeTexture(SpindizzyZoneThemeTexture*);
  void removeColour(SpindizzyZoneThemeColour*);
  SpindizzyZoneThemeIcon* createCustomResourceIcon(IResourceBrowser<ISpindizzyZoneTheme>*);
  
  /**********************************\
   * Implements ISpindizzyZoneTheme *
  \**********************************/
  IColour* getColour(SpindizzyZoneThemeColour*);
  void set();
  
  std::string getName();
  void registerElement(SpindizzyZoneThemeTexture*, ITexture*);
  void registerElement(SpindizzyZoneThemeColour*, IColour*);
  std::map<SpindizzyZoneThemeTexture*, ITexture*> getTextureElements();
  std::map<SpindizzyZoneThemeColour*, IColour*> getColourElements();
  std::string getElementName(SpindizzyZoneThemeTexture*);
  std::string getElementName(SpindizzyZoneThemeColour*);
  
  /*********************************************\
   * Implements IResourceUseListener<ITexture> *
  \*********************************************/
  void resourcePendingDestruction(ITexture*, ITexture*);
  void resourceChanged(ITexture*);

  /********************************************\
   * Implements IResourceUseListener<IColour> *
  \********************************************/
  void resourcePendingDestruction(IColour*, IColour*);
  void resourceChanged(IColour*);
};

#endif
