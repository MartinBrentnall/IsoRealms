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
#ifndef INTEGER_FIELD_H
#define INTEGER_FIELD_H

#include <cfloat>
#include <GL/glew.h>
#include <iostream>
#include <limits>
#include <SDL/SDL.h>
#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IllegalStateException.h>
#include <IsoRealms/IValueListener.h>
#include <IsoRealms/Resources/Font/IFont.h>

#include "ISizedComponent.h"
#include "IValueComponent.h"
#include "LookAndFeel.h"

class IntegerField:public IValueComponent<int>,
                   public ISizedComponent {
  private:
  static const unsigned int BLINK_DELAY = 300;

  std::vector<IValueListener<int>*> cListeners;
  static int cDelayUntilBlinkChange;
  static bool cBlinkShowing;

  unsigned int cCaret;
  bool cUpdating;
  bool cHasFocus;
  std::string cInput;

  bool keyDown(SDLKey&, SDLMod&);
  bool mouseButtonDown(SDL_Event&);
  void fireChange();

  void validateValue();
  
  public:
  IntegerField(std::string = "");

  /***********************************\
   * Implements IValueComponent<int> *
  \***********************************/
  void setValue(int);
  int getValue();
  void addValueListener(IValueListener<int>*);

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
