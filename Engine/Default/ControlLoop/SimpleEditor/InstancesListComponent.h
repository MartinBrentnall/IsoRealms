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
#ifndef INSTANCES_LIST_COMPONENT_H
#define INSTANCES_LIST_COMPONENT_H

#include <GL/gl.h>
#include <iostream>
#include <SDL/SDL.h>
#include <string>
#include <vector>

#include "../../../../Global/AbstractRectangularComponent.h"
#include "../../../../Global/ElementSetRegistry.h"
#include "../../../../Global/IFont.h"

class InstancesListComponent:public AbstractRectangularComponent {
  private:
  static IFont* cFont;

  ElementSetRegistry* cElementSetRegistry;

//  IMovableComponent* cParent;
  std::vector<std::string> cInstances;
  unsigned int cSelectedInstance;

  bool keyDown(SDLKey&);

  public:
  static void setFont(IFont*);

  InstancesListComponent(ElementSetRegistry*);

  IElementSet* getSelectedElementSet();

  void update(int);
  void render();
  bool input(SDL_Event&);
};

#endif
