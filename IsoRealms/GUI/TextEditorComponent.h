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
#ifndef TEXT_EDITOR_COMPONENT_H
#define TEXT_EDITOR_COMPONENT_H

#include <algorithm>
#include <cfloat>
#include <GL/glew.h>
#include <iostream>
#include <SDL/SDL.h>
#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IllegalStateException.h>
#include <IsoRealms/IStringListener.h>
#include <IsoRealms/Resources/Font/IFont.h>

#include "ISizedComponent.h"
#include "IStringValueComponent.h"
#include "LookAndFeel.h"

class TextEditorComponent:public IStringValueComponent,
                          public ISizedComponent {
  private:
  static const unsigned int BLINK_DELAY = 300;
  static const double CARET_X_NOT_ASSIGNED;

  std::vector<IStringListener*> cListeners;
  static int cDelayUntilBlinkChange;
  static bool cBlinkShowing;

  unsigned int cCaretChar;
  unsigned int cCaretLine;
  unsigned int cSelectionChar;
  unsigned int cSelectionLine;
  bool cUpdating;
  bool cHasFocus;
  std::vector<std::string> cInput;
  double cCaretX;

  bool keyDown(SDLKey&, SDLMod&);
  bool mouseButtonDown(SDL_Event&);
  void fireChange();
  void updateCaretX();
  void calculateCaretChar();
  void updateSelection(SDLMod&);

  public:
  TextEditorComponent(std::string = "");

  void setText(std::string);
  std::string getText();
  void addStringListener(IStringListener*);

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
