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
#ifndef STRING_TO_HUD_H
#define STRING_TO_HUD_H

#include <string>

#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/HUDComponents/IHUDComponentFactory.h>
#include <IsoRealms/Resources/HUDComponents/IHUDGameComponent.h>
#include <IsoRealms/Resources/String/IString.h>
#include <IsoRealms/SingleResourceTypeModule.h>

class StringToHUD:public IHUDComponentFactory,
                  public IHUDGameComponent {
  private:
  IString* cText;
  IFont* cFont;

  public:
  StringToHUD();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent();

  /********************************\
   * Implements IHUDGameComponent *
  \********************************/
  void render(float, float);
  void update(unsigned int);
  float getAspectRatio();
  
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
