/*
 * Copyright 2009 Martin Brentnall
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
#ifndef TEXT_FIELD_COMPONENT_H
#define TEXT_FIELD_COMPONENT_H

#include <GL/gl.h>
#include <iostream>
#include <SDL/SDL.h>
#include <string>

#include "../../../../Global/IFont.h"
#include "../../../../Global/IllegalStateException.h"
#include "../../../../Global/IRectangularComponent.h"

class TextFieldComponent:public IRectangularComponent {
  private:
  static IFont* cFont;

  IRectangularComponent* cRelative;
  IRectangularComponent::Edge cEdge;
  float cOffset;
  float cWidth;
  unsigned int cCaret;
  std::string cInput;

  bool keyDown(SDLKey&, SDLMod&);

  public:
  void static setFont(IFont*);

  TextFieldComponent(IRectangularComponent*, IRectangularComponent::Edge, float, float);

  std::string getText();

  void update(int);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);

  /**************************************************************************\
   * Implemented methods of IRectangularComponent.h                         *
  \**************************************************************************/
  float getX();
  float getY();
  float getWidth();
  float getHeight();
};

#endif
