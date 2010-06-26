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
#ifndef CHOOSE_ELEMENTS_COMPONENT_H
#define CHOOSE_ELEMENTS_COMPONENT_H

#include <iostream>
#include <GL/gl.h>
#include <SDL/SDL.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IHUDComponent.h>
#include <IsoRealms/ElementSetRegistry.h>
#include <IsoRealms/ScreenConfiguration.h>

#include "ChooseElementsComponentRow.h"

class ChooseElementsComponent:public IHUDComponent {
  private:
  std::vector<ChooseElementsComponentRow*> cElementRows;
  unsigned int cSelectedSet;
  float cAnimation;

  IComponentContainer* cComponentContainer;

  bool keyDown(SDLKey&);
  bool mouseButtonDown(SDL_Event&);

  class CloseCommand:public ICommand {
    private:
    ChooseElementsComponent* cParent;

    public:
    CloseCommand(ChooseElementsComponent*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  void close();

  public:
  ChooseElementsComponent(IComponentContainer*, ElementSetRegistry*);

  void addElementSelectionListener(IElementSelectionListener*);

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void update(int);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);
};

#endif
