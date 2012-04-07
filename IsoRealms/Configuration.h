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

extern "C" {
  #include "lua.h"
  #include "lualib.h"
}
#include <luabind/luabind.hpp>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <string>

#include "DOMNodeWrapper.h"
#include "DOMNodeWriter.h"
#include "Hacks.h"
#include "IEngine.h"
#include "IInteger.h"
#include "ILuaFunctionArgument.h"
#include "InitException.h"
#include "ISound.h"
#include "ParseException.h"
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

  /**
   * Singleton instance of the configuration.
   */
  static Configuration* cInstance;

  /**
   * Logic of the switch states.
   */
//  SwitchLogic* cSwitchLogic;
  ScreenConfiguration* cScreenConfiguration;
  lua_State *cLuaState;
  IEngine* cEngine;

  /**
   * Constructor of the configuration.
   *
   * @param string  Name of the file from which to load the user
   *                settings.
   * @param string  Name of the file from which to load the engine
   *                configuration.
   */
  Configuration(std::string, std::string);

  /**
   * Parse the user settings from the specified node.
   *
   * @param  DOMNodeWrapper*  The node from which to parse the user settings.
   */
  void parseSettings(DOMNodeWrapper*);

  void parseSettings();
  
  void createSettings();
  
  /**
   * Parse the engine configuration from the specified node.
   *
   * @param  DOMNodeWrapper*  The node from which to parse the engine
   *                          configuration.
   */
  void parseConfiguration(DOMNodeWrapper*);

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

  /**
   * Get the game engine.
   *
   * @return  The game engine.
   */
  IEngine* getEngine();

  void registerScript(const std::string&);
  
  void executeScript(const std::string&, std::vector<ILuaFunctionArgument*>);
  
  void save();
};

#endif
