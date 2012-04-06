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
#ifndef CHOOSE_ELEMENTS_COMPONENT_ROW_H
#define CHOOSE_ELEMENTS_COMPONENT_ROW_H

#include <GL/glew.h>
#include <SDL/SDL.h>

#include <IsoRealms/ICommand.h>
#include <IsoRealms/IElementFactory.h>
#include <IsoRealms/IElementSet.h>

#include "IElementSelectionListener.h"

class ChooseElementsComponentRow {
  private:
  ICommand* cCloseCommand;
  std::vector<IElementSelectionListener*> cElementSelectionListeners;
  std::vector<IElementFactory*> cElements;
  unsigned int cSelectedElement;
  float cAnimation;

  bool keyDown(SDLKey&);
 
  public:
  ChooseElementsComponentRow(ICommand*, IElementSet*);
  void addElementSelectionListener(IElementSelectionListener*);

  bool testClick(float);

  void update(int);
  void render();
  bool input(SDL_Event&);
};

#endif
