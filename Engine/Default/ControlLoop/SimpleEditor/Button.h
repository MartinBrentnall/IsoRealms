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
#ifndef BUTTON_H
#define BUTTON_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <string>

#include "../../../../Global/AbstractRectangularComponent.h"
#include "../../../../Global/Configuration.h"
#include "../../../../Global/ICommand.h"
#include "../../../../Global/ISizedComponent.h"
#include "../../../../Global/IFont.h"
#include "../../../../Global/IllegalStateException.h"
#include "../../../../Global/ScreenConfiguration.h"

class Button:public ISizedComponent {
  private:
  static IFont* cFont;
  bool cButtonPressed;
  bool cHovering;
  ICommand* cCommand;
  std::string cText;

  bool mouseMotion(SDL_Event&);
  bool mouseButtonUp(SDL_Event&);
  bool mouseButtonDown(SDL_Event&);

  public:
  void static setFont(IFont*);

  Button(IComponentBoundsCalculator*, ICommand*, std::string);

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
