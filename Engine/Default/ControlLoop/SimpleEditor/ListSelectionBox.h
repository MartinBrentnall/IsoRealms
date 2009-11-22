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
#ifndef LIST_SELECTION_BOX_H
#define LIST_SELECTION_BOX_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <vector>
#include <string>

#include "../../../../Global/IComponentContainer.h"
#include "../../../../Global/IFont.h"
#include "../../../../Global/RectangleComponent.h"
#include "../../../../Global/System.h"

#include "IItemSelectedCommand.h"
#include "IListItem.h"

class ListSelectionBox:public RectangleComponent {
  private:
  IItemSelectedCommand* cListener;
  static IFont* cFont;
  unsigned int cSelectedItem;
  std::vector<std::string>* cItems;

  bool keyDown(SDLKey&);

  public:
  static void setFont(IFont*);

  ListSelectionBox(IComponentContainer*, IItemSelectedCommand*, std::vector<IListItem*>&, float, float);
  ListSelectionBox(IComponentContainer*, IItemSelectedCommand*, std::vector<std::string>&, float, float);

  /**************************************************************************\
   * Implemented methods of RectangleComponent.h                            *
  \**************************************************************************/
  void updateContent(int);
  void renderContent();
  bool inputContent(SDL_Event&);
};

#endif
