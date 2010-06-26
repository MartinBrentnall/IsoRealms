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
      std::string mEngineLocation = System::getConfigurationResource("Engine/" + mEngineName + "/Engine");
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
    } else if (mValueAsString == "FontEngine") {
      std::string mFontEngineLocation = mNode->getAttribute("name");
      std::string mEngineLocation = System::getConfigurationResource("FontEngine/" + mFontEngineLocation + "/FontEngine");
      void* mFontEngineSO = dlopen(mEngineLocation.c_str(), RTLD_LAZY);
      if (!mFontEngineSO) {
        throw InitException("Cannot load library: " + std::string(dlerror()));
      }
      createFontEngine* createFontEngineFunction = cast_voidptr_to_funcptr<createFontEngine*>(dlsym(mFontEngineSO, "create"));
      const char* mDlsymError = dlerror();
      if (mDlsymError) {
        throw InitException("Cannot load symbol: " + std::string(mDlsymError));
      }
      IFontEngine* mFontEngine = createFontEngineFunction(mNode);
      GlobalConfiguration::setFontEngine(mFontEngine);
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

ScreenConfiguration* Configuration::getScreenConfiguration() {
  if (cScreenConfiguration == NULL) {
    System::checkUserDataDirectory();
    if (System::configurationFileExists()) {
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
    } else {
      DOMNodeWriter* mRootNode = new DOMNodeWriter("IsoRealmsConfiguration");
      DOMNodeWriter* mScreenNode = mRootNode->addBranch("Screen");
      DOMNodeWriter* mFullscreenNode = mScreenNode->addBranch("Fullscreen");
      mFullscreenNode->addText("true");
      DOMNodeWriter* mScreenSizeNode = mScreenNode->addBranch("Size");
      DOMNodeWriter* mScreenWidthNode = mScreenSizeNode->addBranch("Width");
      mScreenWidthNode->addText("1024");
      DOMNodeWriter* mScreenHeightNode = mScreenSizeNode->addBranch("Height");
      mScreenHeightNode->addText("768");
      DOMNodeWriter* mScreenDepthNode = mScreenNode->addBranch("Depth");
      mScreenDepthNode->addText("24");
      mRootNode->save(cSettingsFile);
    }
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
