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
#ifndef DYNAMIC_MENU_ITEMS_H
#define DYNAMIC_MENU_ITEMS_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <string>
#include <vector>

#include <IsoRealms/ICommand.h>
#include <IsoRealms/Input/KeyStates.h>
#include <IsoRealms/Resources/Font/IFont.h>

#include "IMenuEntry.h"
#include "LookAndFeel.h"

class DynamicMenuItems:public IMenuEntry {
  private:
  std::vector<std::pair<std::string, ICommand*> > cItems;
  unsigned int cSelected;

  public:

  /**
   * Construct a new menu item with the specified command.
   */
  DynamicMenuItems();

  void addItem(std::string, ICommand*);
  
  void render(bool, float, float);

  float getWidth();
  float getHeight();

  bool input(SDL_Event&);
  void execute();
  bool contains(float, float, float, float);
  bool testClick(float, float, float, float);
  bool isSubMenuCommand();
};

#endif
