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
#ifndef SELECTION_LIST_H
#define SELECTION_LIST_H

#include <iostream>

#include <IsoRealms/ICommand.h>
#include <IsoRealms/System.h>

/**
 * This class will dynamically populate the front-end menu with a list of files
 * from a specific folder.
 */
class SelectionList:public ICommand {
  private:
  std::string cDir;
  std::string cFilter;
  std::vector<std::string>* cActiveMenu;
  unsigned int* cSelectedMenuItem;
  std::string** cMenuLocation;
  std::string cDestinationLocation;

  public:
  /**
   * 
   * @param vector<string>&  Active menu to populate.
   * @param string  The directory to list files from.
   * @param string  Filename filter
   * @param int&    The menu item selection number.
   */
  SelectionList(std::vector<std::string>&, std::string, std::string, unsigned int&, std::string*&, std::string);

  /***********************\
   * Implements ICommand *
  \***********************/
  void execute();    
};

#endif
