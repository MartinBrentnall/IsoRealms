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
#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <string>

#include <IsoRealms/GUI/LookAndFeel.h>
#include <IsoRealms/IFont.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/KeyStates.h>

class MenuItem {
  private:
  char cAccelerator;
  std::string::size_type cAcceleratorIndex;
  std::string cText;
  ICommand* cCommand;
  float cXOffset;
  float cYOffset;

  public:

  /**
   * Construct a new menu item with the specified command.
   */
  MenuItem(std::string, ICommand*, float, float);

  void render(bool);

  float getWidth();
  float getHeight();

  bool input(SDL_Event&);
  void execute();
  bool contains(float x, float y);
  bool testClick(float x, float y);
};

#endif
