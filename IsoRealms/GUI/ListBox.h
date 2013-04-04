/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef LIST_BOX_H
#define LIST_BOX_H

#include <GL/glew.h>
#include <string>
#include <vector>

#include "../Configuration.h"
#include "../MultipleClickDetector.h"
#include "IListBoxListener.h"
#include "ISizedComponent.h"
#include "LookAndFeel.h"

class ListBox:public ISizedComponent {
  private:
  MultipleClickDetector cMultipleClickDetector;
  unsigned int cSelectedItem;
  std::vector<std::string> cItems;
  std::vector<IListBoxListener*> cListeners;

  bool keyDown(SDLKey&);
  bool mouseButtonPressed(SDL_Event&);

  public:
  ListBox();

  std::string& getSelectedItem();
  void addItem(const std::string&);
  void addListener(IListBoxListener*);
  void removeItem(const std::string&);
  void clear();

  /******************************\
   * Implements ISizedComponent *
  \******************************/
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  float getWidth();
  float getHeight();
};

#endif
