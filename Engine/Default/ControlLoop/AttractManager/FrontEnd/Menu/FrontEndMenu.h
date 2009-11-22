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
#ifndef FRONT_END_MENU_H
#define FRONT_END_MENU_H

#include <iostream>
#include <map>
#include <vector>
#include <GL/gl.h>
#include <SDL/SDL.h>

#include "EditMapCommand.h"
#include "EnterMenuCommand.h"
#include "SelectionList.h"

#include "../../../../../../Global/CommandManager.h"
#include "../../../../../../Global/DOMNodeWrapper.h"
#include "../../../../../../Global/GlobalConfiguration.h"
#include "../../../../../../Global/IFont.h"
#include "../../../../../../Global/IFontEngine.h"
#include "../../../../../../Global/ICommand.h"

#include "../../IFrontEnd.h"

/**
 * This attract scene displays the title logo for a limited time.
 */
class FrontEndMenu:public IFrontEnd {
  private:

  // The following variables control idle timeout
  /**
   * Maximum idle time to elapse before the front-end automatically terminates.
   */
  static int MAX_IDLE_TIME;

  /**
   * Idle time elapse without user input.
   */
  int cIdleTime;

  // The following variables control animation transitions and presentation.
  /**
   * Control variable for animation transitions.
   */
  int cStage;

  /**
   * Control variable for 
   */
  float cFade;

  /**
   * Font to use for drawing the menu.
   */
  IFont* cFont;

  /**
   * Index of the currently selected menu item.
   */
  unsigned int cSelectedItem;

  // TODO: The following variables control...?
  /**
   * Actions may set the new location when moving through menus.
   */
  std::string* cNewLocation;
  std::string cCommandArgument;


  std::vector<std::string> cSubMenus;
  std::vector<std::string>* cActiveMenuItems;
  std::vector<std::string> cCurrentLocation;
  std::map<std::string, std::vector<std::string>* > cMenuItems;
  std::map<std::string, ICommand*> cMenuItemCommands;
  std::map<std::string, ICommand*> cMenuItemArgumentedCommands;
  ICommand* cExitCommand;
  bool cHasExited;

  // Input functions
  void keyDown(SDLKey&);
  void mouseButtonDown();

  // Initialization
  void parseMenu(DOMNodeWrapper*, std::string, std::vector<std::string>*);
  ICommand* parseEventCommand(DOMNodeWrapper* node);

  bool isSubMenu(std::string);
  std::string getActionID(std::string);
  std::string getActionID();
  void executeAction(std::string);

  public:
  FrontEndMenu(DOMNodeWrapper*);

  /**
   * Signal the front-end to terminate.
   */
  void terminate();

  /**************************************************************************\
   * Implemented methods of IFrontEnd.h                                     *
  \**************************************************************************/
  void update(int);
  void render();
  void setActive(bool);
  void input(SDL_Event& event);
  bool hasIdled();
  bool hasExited();
};

#endif
