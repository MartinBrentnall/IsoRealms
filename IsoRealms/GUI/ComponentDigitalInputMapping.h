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
#ifndef COMPONENT_DIGITAL_INPUT_MAPPING_H
#define COMPONENT_DIGITAL_INPUT_MAPPING_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Input/SDLKeyMap.h>
#include <IsoRealms/IValueListener.h>
#include <IsoRealms/GUI/LookAndFeel.h>
#include <IsoRealms/GUI/ISizedComponent.h>
#include <IsoRealms/GUI/IValueComponent.h>

class ComponentDigitalInputMapping:public ISizedComponent {
  private:
  static SDLKeyMap cSDLKeyMap;

  std::vector<SDLKey> cKeys;
  bool cHasFocus;
  float cSize;
  IResourceSelector* cResourceSelector;
  
  void fireChange();
  
  public:
  ComponentDigitalInputMapping(std::vector<SDLKey>, float);

  /***************************************\
   * Implements IComponentSizeCalculator *
  \***************************************/
  float getWidth();
  float getHeight();  

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  virtual void gainedFocus();
  virtual void lostFocus();  
};

#endif

