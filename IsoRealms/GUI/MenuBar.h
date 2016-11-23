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
#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <cctype>
#include <map>
#include <vector>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/LookAndFeel.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IHUDComponent.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/ScreenConfiguration.h>

#include "MenuItem.h"
#include "MenuPopup.h"
#include "PopupMenuCommand.h"

class MenuBar:public IHUDComponent,
              public IMenuContainer,
              public IMenuPopup {
  private:
  IComponentContainer* cComponentContainer;
  std::map<std::string, MenuPopup*> cMenuPopups;
  std::vector<MenuItem*> cMenuItems;
  MenuPopup* cMenuPopupShowing;
  float cWidth;
  unsigned int cSelectedItem;

  /**
   * Parse a menu from the specified node.
   * 
   * @param DOMNodeWrapper*  The node to parse.
   */
  ICommand* parseMenu(DOMNodeWrapper*);

  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(SDL_Event&);
  bool keyDown(SDLKey&);

  public:
  MenuBar(IComponentContainer*, DOMNodeWrapper*, ICommandSource*, IResourceAccessor*);

  DynamicMenuItems* getDynamicMenuItems(const std::string&);
  
  void addCommand(ICommandInfo*);
  void removeCommand(ICommandInfo*);

  float getLeft();
  float getBottom();
  float getRight();
  float getTop();

  /*****************************\
   * Implements IMenuContainer *
  \*****************************/
  void closeMenu(IMenu*);

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  bool contains(float, float);
  
  void popupMenu(IMenuPopup*);
};

#endif
