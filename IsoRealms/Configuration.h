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
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <string>

#include "IEngine.h"
#include "InitException.h"
#include "LuaSupport/ILuaSupport.h"
#include "Persistence/DOMNodeWrapper.h"
#include "Persistence/DOMNodeWriter.h"
#include "Persistence/ParseException.h"
#include "Resources/IResourceRegistry.h"
#include "ScreenConfiguration.h"
#include "System.h"

/**
 * The configuration is responisble for setting up the drivers for the screen,
 * logic, and so on.
 */
class Configuration {
  private:
  std::string cConfigurationFile;
  std::string cSettingsFile;
  ILuaSupport* cLuaSupport;

  /**
   * Singleton instance of the configuration.
   */
  static Configuration* cInstance;

  /**
   * Logic of the switch states.
   */
//  SwitchLogic* cSwitchLogic;
  ScreenConfiguration* cScreenConfiguration;
  IEngine* cEngine;

  /**
   * Constructor of the configuration.
   *
   * @param string  Name of the file from which to load the user
   *                settings.
   * @param string  Name of the file from which to load the engine
   *                configuration.
   */
  Configuration(std::string);

  /**
   * Parse the user settings from the specified node.
   *
   * @param  DOMNodeWrapper*  The node from which to parse the user settings.
   */
  void parseSettings(DOMNodeWrapper*);

  void parseSettings();
  
  void createSettings();
  
  public:
    
  /**
   * Get the singleton instance of the Configuration.
   *
   * @return  The singleton instance of the configuration.
   */
  static Configuration* getInstance();

  /**
   * Get the screen configuration.
   *
   * @return  The screen configuration.
   */
  ScreenConfiguration* getScreenConfiguration();

  void setViewPort();
  
  void save();

  void setLuaSupport(ILuaSupport*);
  
  void registerScript(const std::string&);
  
  void setGlobalVariable(IArgumentDefinition*);
  
  void executeScript(const std::string&, std::vector<IArgumentValue*>);

  IArgumentValue* getArgumentValue(DOMNodeWrapper* node, IResourceAccessor* resources);
  
  void enableLuaSupport(InitLuaFunction*);  
};

#endif
