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
#ifndef TEST_SCREEN_H
#define TEST_SCREEN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <sstream>
#include <vector>

#include <IsoRealms/Resources/Font/IFont.h>

#include "IApplicableItem.h"
#include "IFrontEndMenu.h"
#include "IMenuStack.h"

class TestScreen:public IFrontEndMenu {
  private:
  std::vector<IApplicableItem*> cApplicableItems;
  int cTimeOut;
  bool cConfirmed;
  IMenuStack* cMenuStack;

  bool keyDown(SDLKey&);
  
  public:
  TestScreen(std::vector<IApplicableItem*>, IMenuStack*);

  /****************************\
   * Implements IFrontEndMenu *
  \****************************/
  bool input(SDL_Event&);
  void render(float, IFont*);
  void update(unsigned int);
  void cancelled();
};

#endif
