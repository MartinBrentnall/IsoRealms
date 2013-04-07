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
#ifndef OPTION_MENU_ITEM_H
#define OPTION_MENU_ITEM_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <vector>

#include "IApplicableItem.h"
#include "IFrontEndMenuItem.h"
#include "IOption.h"

class OptionMenuItem:public IFrontEndMenuItem,
                     public IApplicableItem {
  private:
  std::string cName;
  std::vector<IOption*> cOptions;
  unsigned int cSelectedOption;
  unsigned int cPreviousOption;

  public:
  OptionMenuItem(const std::string&, std::vector<IOption*>, unsigned int);
  
  /******************************\
   * Implements IApplicableItem *
  \******************************/
  void apply();
  void reset();
  void save();
    
  /********************************\
   * Implements IFrontEndMenuItem *
  \********************************/
  void render(int, float, IFont*, bool);
  FocusAction keyDown(SDLKey&);
};

#endif
