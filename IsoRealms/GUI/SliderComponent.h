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
#ifndef SLIDER_COMPONENT_H
#define SLIDER_COMPONENT_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "../Configuration.h"
#include "../MiscFunctions.h"
#include "IFloatListener.h"
#include "ISizedComponent.h"
#include "ISliderRenderer.h"

class SliderComponent:public ISizedComponent {
  private:
  float cValue;
  bool cSliderUpdating;
  bool cDragging;
  ISliderRenderer* cRenderer;
  std::vector<IFloatListener*> cValueListeners;
  
  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(Uint16);
  bool keyDown(SDLKey, SDLMod);
  void fireChange();
  
  public:
  SliderComponent();
  
  void setRenderer(ISliderRenderer*);
    
  void setValue(float);
  float getValue();
  
  void addFloatListener(IFloatListener*);
  
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
};

#endif
