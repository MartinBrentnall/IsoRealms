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

#include "../../Font/IFontPlugin.h"
#include "../../HUD/IHUD.h"

#include "../IStringProcessor.h"

class StringToHUD:public IStringProcessor,
                  public IHUDComponentFactory,
                  public IHUDGameComponent {
  private:
  std::string* cText;
  IFont* cFont;
  IHUD* cHUD;

  public:
  StringToHUD();

  /*******************************\
   * Implements IStringProcessor *
  \*******************************/
  void registerString(std::string*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  
  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent(const std::string&);

  /********************************\
   * Implements IHUDGameComponent *
  \********************************/
  void render(float, float);
  void update(int);
  float getAspectRatio();
};

#endif
