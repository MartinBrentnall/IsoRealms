/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef TEXT_LABEL_COMPONENT_H
#define TEXT_LABEL_COMPONENT_H

#include <GL/glew.h>
#include <string>

#include "../IFont.h"

#include "ISizedComponent.h"
#include "LookAndFeel.h"

class TextLabelComponent:public ISizedComponent {
  private:
  std::string cLabel;

  public:
  TextLabelComponent(std::string);
  
  /***************************************\
   * Implements IComponentSizeCalculator *
  \***************************************/
  float getWidth();
  float getHeight();  

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void update(int);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);
};

#endif
