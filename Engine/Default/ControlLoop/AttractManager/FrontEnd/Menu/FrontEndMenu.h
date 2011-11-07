/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef FRONT_END_MENU_H
#define FRONT_END_MENU_H

#include <SDL/SDL.h>
#include <string>
#include <vector>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/InputCommands.h>
#include <IsoRealms/System.h>

#include "BooleanMenuItem.h"
#include "DigitalInputMenuItem.h"
#include "EnterMenuCommand.h"
#include "FocusAction.h"
#include "FrontEndMenuItem.h"
#include "IApplicableItem.h"
#include "IFrontEndCommands.h"
#include "IFrontEndMenu.h"
#include "IFrontEndMenuItem.h"
#include "IMenuStack.h"
#include "OptionMenuItem.h"
#include "ResolutionOption.h"

class FrontEndMenu:public IFrontEndMenu {
  private:
  std::string cMenuName;
  std::vector<IFrontEndMenuItem*> cMenuItems;
  std::vector<IApplicableItem*> cApplicableItems;
  unsigned int cSelectedItem;
  
  void keyDown(SDLKey&);
  
  std::vector<IFrontEndMenuItem*> parseControlConfig(const std::string&);
  
  bool isNumeric(const std::string&, std::size_t, std::size_t);
  std::size_t getFormatChars(const std::string&, std::size_t);
  std::string format(const std::string&, std::vector<std::string>);
  
  public:
  FrontEndMenu(IFrontEndCommands*, IMenuStack*, DOMNodeWrapper*, const std::string&, std::vector<std::string>);
  FrontEndMenu(IFrontEndCommands*, DOMNodeWrapper*);
  
  /****************************\
   * Implements IFrontEndMenu *
  \****************************/
  bool input(SDL_Event&);
  void render(float, IFont*);
  void update(unsigned int);
  void applyAll();
};

#endif
