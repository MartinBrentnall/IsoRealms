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

Configuration::Configuration(std::string filename, std::string engineFileName) {
  cLuaState = luaL_newstate();
  luabind::open(cLuaState);
  luaL_openlibs(cLuaState);
  
  luabind::module(cLuaState) [
    luabind::class_<ISound>("Sound")
       .def("play", &ISound::play) 
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IInteger>("Integer")
       .def("setValue", &IInteger::setValue)
       .def("getValue", &IInteger::getValue)
  ];
      
  cConfigurationFile = engineFileName;
  cSettingsFile = filename;
  cScreenConfiguration = NULL;
  cEngine = NULL;
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

void Configuration::parseConfiguration(DOMNodeWrapper *node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Engine") {
      std::string mEngineName = mNode->getAttribute("name");
      std::string mEngineLocation = System::getConfigurationResource("Engine/" + mEngineName + "/libEngine");
      void* mEngineSO = dlopen(mEngineLocation.c_str(), RTLD_LAZY);
      if (!mEngineSO) {
        throw InitException("Cannot load library: " + std::string(dlerror()));
      }
      createEngine* createEngineFunction = cast_voidptr_to_funcptr<createEngine*>(dlsym(mEngineSO, "create"));
      const char* mDlsymError = dlerror();
      if (mDlsymError) {
        throw InitException("Cannot load symbol: " + std::string(mDlsymError));
      }
      cEngine = createEngineFunction(mNode);
    }
  }
}

Configuration* Configuration::getInstance() {
  if (cInstance == NULL) {
    std::string mSettingsFileLocation = System::getSettingsFileLocation();
    std::string mConfigurationFileLocation = System::getConfigurationFileLocation();
    cInstance = new Configuration(mSettingsFileLocation, mConfigurationFileLocation);
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

IEngine* Configuration::getEngine() {
  try {
    DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(cConfigurationFile);
    for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
      DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
      std::string mValue = mNode->getNodeName();
      if (mValue == "IsoRealmsConfiguration") {
        parseConfiguration(mNode);
      }
    }
  } catch (ParseException &e) {
    throw InitException("Parsing exception occurred during initalization\n" + e.getMessage());
  }
  return cEngine;
}

void Configuration::registerScript(const std::string& script) {
  luaL_dostring(cLuaState, script.c_str());
}

void Configuration::executeScript(const std::string& script, std::vector<ILuaFunctionArgument*> arguments) {
  for (unsigned int i = 0; i < arguments.size(); i++) {
    std::string mArgFunction = script + "_arg" + Utils::toString(i);
    arguments[i]->setArgument(cLuaState, mArgFunction);
  }
  luabind::call_function<void>(cLuaState, script.c_str());
}

void Configuration::save() {
  DOMNodeWriter* mConfigurationNode = new DOMNodeWriter("IsoRealmsSettings");
  cScreenConfiguration->save(mConfigurationNode);
  mConfigurationNode->save(cSettingsFile);
}

