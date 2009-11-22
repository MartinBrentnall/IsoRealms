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
#ifndef REGISTER_PLUGIN_INSTANCE_COMMAND_H
#define REGISTER_PLUGIN_INSTANCE_COMMAND_H

#include <string>
#include <iostream>

#include "../../../../Global/ICommand.h"
#include "../../../../Global/PluginRegistry.h"

class RegisterPluginInstanceCommand:public ICommand {
  private:
  PluginRegistry* cPluginRegistry;
  std::string cPluginType;
  std::string cImplementationName;
  std::string cInstanceName;  

  public:
  RegisterPluginInstanceCommand(PluginRegistry*, std::string&, std::string&, std::string&);

  /*************************************************************************\
   * Implemented methods of ICommand.h                                     *
  \*************************************************************************/
  void execute();  
};

#endif
