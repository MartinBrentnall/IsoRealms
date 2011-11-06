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
#ifndef FRONT_END_H
#define FRONT_END_H

#include <iostream>
#include <map>
#include <vector>
#include <GL/gl.h>
#include <SDL/SDL.h>

#include <IsoRealms/CommandManager.h>
#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/IFont.h>
#include <IsoRealms/ICommand.h>

#include "../../IFrontEnd.h"

#include "ArgumentedCommand.h"
#include "EditMapCommand.h"
#include "FrontEndMenu.h"
#include "IMenuStack.h"

/**
 * This attract scene displays the title logo for a limited time.
 */
class FrontEnd:public IFrontEnd,
               public IMenuStack,
               public IFrontEndCommands {
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

  IEngineArguments* cEngineArguments;
  
  // TODO: The following variables control...?
  /**
   * Index of the currently selected menu item.
   */
  std::vector<FrontEndMenu*> cActiveMenu;
  ICommand* cExitCommand;
  bool cHasExited;

  bool keyDown(SDLKey&);
  
  public:
  FrontEnd(DOMNodeWrapper*, IFont*, IEngineArguments*);

  /**
   * Signal the front-end to terminate.
   */
  void terminate();

  /************************\
   * Implements IFrontEnd *
  \************************/
  void update(int);
  void render();
  void setActive(bool);
  void input(SDL_Event&);
  bool hasIdled();
  bool hasExited();
  
  /********************************\
   * Implements IFrontEndCommands *
  \********************************/
  ICommand* parseCommand(DOMNodeWrapper*);
  ICommand* parseArgumentCommand(DOMNodeWrapper*, const std::string&);

  /*************************\
   * Implements IMenuStack *
  \*************************/
  void push(FrontEndMenu*);
};

#endif
