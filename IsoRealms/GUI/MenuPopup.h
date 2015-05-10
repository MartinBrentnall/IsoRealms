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
#ifndef MENU_POPUP_H
#define MENU_POPUP_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <map>
#include <vector>

#include <IsoRealms/Resources/Script/IScriptCall.h>

#include "../Configuration.h"
#include "../Persistence/DOMNodeWrapper.h"
#include "../IComponentContainer.h"
#include "../ICommand.h"
#include "../ICommandInfo.h"
#include "../ICommandSource.h"
#include "../ScreenConfiguration.h"

#include "IMenuContainer.h"
#include "IMenu.h"
#include "IMenuPopup.h"
#include "MenuItem.h"
#include "PopupMenuCommand.h"

class MenuPopup:public IMenu,
                public IMenuPopup {
  private:
  std::map<std::string, MenuPopup*> cMenuPopups;
  std::map<std::string, MenuItem*> cMenuItemsByName;
  std::vector<MenuItem*> cMenuItems;
  MenuPopup* cSubMenuShowing;
  float cX;
  float cY;
  IMenuContainer* cParent;
  unsigned int cSelectedItem;

  ICommand* parseCommand(DOMNodeWrapper*, ICommandSource*, IResourceAccessor*);
  
  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(SDL_Event&);
  bool keyDown(SDLKey&);

  void addMenuItem(const std::string&, const std::string&, ICommand*, bool);

  public:
  MenuPopup(DOMNodeWrapper*, IMenuContainer*, float, float, IComponentContainer*, ICommandSource*, IResourceAccessor*);
  MenuPopup(IMenuContainer*, float, float);

  void addCommand(std::vector<std::string>, ICommandInfo*);
  void setPosition(float, float);

  float getX();
  float getY();
  float getWidth();
  float getHeight();

  void render();
  bool input(SDL_Event&);

  bool contains(float, float);
  bool setComponentAction(const std::string&, ICommand*);
  bool addComponentAction(const std::string&, const std::string&, ICommand*);
  
  void popupMenu(IMenuPopup*);
};

#endif
