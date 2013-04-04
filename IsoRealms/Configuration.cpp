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
#include "Configuration.h"

Configuration* Configuration::cInstance = NULL;

Configuration::Configuration(std::string filename) {
  cSettingsFile = filename;
  cScreenConfiguration = NULL;
  cEngine = NULL;
}

void Configuration::setLuaSupport(ILuaSupport* luaSupport) {
  cLuaSupport = luaSupport;
}

void Configuration::parseSettings(DOMNodeWrapper *node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Screen") {
      cScreenConfiguration = new ScreenConfiguration(mNode);
      cScreenConfiguration->openScreen("Iso Realms");
    }
  }
}

Configuration* Configuration::getInstance() {
  if (cInstance == NULL) {
    std::string mSettingsFileLocation = System::getSettingsFileLocation();
    cInstance = new Configuration(mSettingsFileLocation);
  }
  return cInstance;
}

void Configuration::parseSettings() {
  try {
    DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(cSettingsFile);
    for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
      DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
      std::string mValue = mNode->getNodeName();
      if (mValue == "IsoRealmsSettings") {
        parseSettings(mNode);
      }
    }
  } catch (ParseException &e) {
    throw InitException("Parsing exception occurred during initalization\n" + e.getMessage());
  }
}

ScreenConfiguration* Configuration::getScreenConfiguration() {
  if (cScreenConfiguration == NULL) {
    System::checkUserDataDirectory();
    if (System::configurationFileExists("settings.xml")) {
      parseSettings();
    } else {
      cScreenConfiguration = new ScreenConfiguration();
      cScreenConfiguration->openScreen("Iso Realms");
    }
  }
  
  if (cScreenConfiguration == NULL) {
    std::cout << "Screen not initialised!" << std::endl;
    exit(1);
  }
  return cScreenConfiguration;
}

void Configuration::setViewPort() {
  cScreenConfiguration->setViewPort();
}

void Configuration::registerScript(const std::string& script) {
  cLuaSupport->registerScript(script);
}

void Configuration::setGlobalVariable(IArgumentDefinition* argument) {
  cLuaSupport->setGlobalVariable(argument);
}

void Configuration::executeScript(const std::string& script, std::vector<ILuaFunctionArgument*> arguments) {
  cLuaSupport->executeScript(script, arguments);
}

IArgumentDefinition* Configuration::createArgumentDefinition(DOMNodeWrapper* node, IResourceAccessor* resources) {
  return cLuaSupport->createArgumentDefinition(node, resources);
}

IArgumentSource* Configuration::createArgument(DOMNodeWrapper* node, IResourceAccessor* resources) {
  return cLuaSupport->createArgument(node, resources);
}

void Configuration::enableLuaSupport(InitLuaFunction* initLuaFunction, IRuntimeContext* runtimeContext) {
  cLuaSupport->enableLuaSupport(initLuaFunction, runtimeContext);
}

void Configuration::save() {
  DOMNodeWriter* mConfigurationNode = new DOMNodeWriter("IsoRealmsSettings");
  cScreenConfiguration->save(mConfigurationNode);
  mConfigurationNode->save(cSettingsFile);
}

