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
#ifndef DROP_DOWN_LIST_COMPONENT_H
#define DROP_DOWN_LIST_COMPONENT_H

#include <cfloat>
#include <GL/glew.h>
#include <iostream>
#include <SDL/SDL.h>
#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/IValueComponent.h>
#include <IsoRealms/IllegalStateException.h>
#include <IsoRealms/IValueListener.h>
#include <IsoRealms/MultipleClickDetector.h>
#include <IsoRealms/Resources/Font/IFont.h>

#include "ISizedComponent.h"
#include "IValueComponent.h"
#include "LookAndFeel.h"

class DropDownListComponent:public ISizedComponent,
                            public IValueComponent<unsigned int> {
  private:
  bool cUpdating;
  bool cHasFocus;
  std::vector<std::string> cItems;
  unsigned int cSelected;
  IComponentContainer* cWindowWorkspace;
  MultipleClickDetector cMultipleClickDetector;
  std::vector<IValueListener<unsigned int>*> cValueChangeListeners;

  bool keyDown(SDLKey&, SDLMod&);
  bool mouseButtonDown(SDL_Event&);

  class ListPopupBounds:public IComponentBoundsCalculator {
    private:
    DropDownListComponent* cParent;
      
    public:
    ListPopupBounds(DropDownListComponent*);
      
    float getLeft();
    float getRight();
    float getBottom();
    float getTop();
  };
  
  class ListPopup:public AbstractRectangularComponent {
    private:
    DropDownListComponent* cParent;
    
    bool keyDown(SDLKey&, SDLMod&);
    bool mouseButtonDown(SDL_Event&);
    
    public:
    ListPopup(DropDownListComponent*, IComponentBoundsCalculator*);
    
    bool contains(float, float);
    
    /****************************\
     * Implements IHUDComponent *
    \****************************/
    void update(unsigned int);
    void render();
    bool input(SDL_Event&);
  };
  
  void fireValueChange();
  
  public:
  DropDownListComponent(std::vector<std::string>, unsigned int, IComponentContainer*);

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
  void gainedFocus();
  void lostFocus();

  /********************************************\
   * Implements IValueComponent<unsigned int> *
  \********************************************/
  void setValue(unsigned int);
  unsigned int getValue();
  void addValueListener(IValueListener<unsigned int>*);
};

#endif

