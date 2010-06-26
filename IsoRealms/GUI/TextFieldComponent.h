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

#include "../IFont.h"
#include "../IllegalStateException.h"

#include "ISizedComponent.h"
#include "LookAndFeel.h"

class TextFieldComponent:public ISizedComponent {
  private:
  static const unsigned int BLINK_DELAY = 100;

  static int cDelayUntilBlinkChange;
  static bool cBlinkShowing;

  unsigned int cCaret;
  std::string cInput;

  bool keyDown(SDLKey&, SDLMod&);

  public:
  TextFieldComponent(std::string = "");

  void setText(std::string);
  std::string getText();

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
};

#endif
