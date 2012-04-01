/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "InputCommands.h"

InputCommands::InputCommands() {
  // Nothing to do
}

InputCommands::InputCommands(const std::string& file) {
  std::string mConfigDir = "Projects/" + file;
  System::makeUserDataDirectory(mConfigDir);
  std::string mConfigFile = mConfigDir + "/controls.config";
  std::string mConfigFileResource = System::getUserResource(mConfigFile);
  std::cout << "Checking configuration at: " << mConfigFileResource << std::endl;
  if (!System::configurationFileExists(mConfigFile)) {
    DOMNodeWriter* mNode = new DOMNodeWriter("InputConfiguration");
    mNode->save(mConfigFileResource);
  }
  DOMNodeWrapper* mControlConfigNode = new DOMNodeWrapper(mConfigFileResource);
  std::vector<std::string> mEmptyVector;
  loadConfiguration(mControlConfigNode, mEmptyVector, NULL);
}

DigitalInput* InputCommands::getDigitalInput(const std::string& name) {
  std::map<std::string, DigitalInput*>::iterator i = cDigitalInputs.find(name);
  if (i != cDigitalInputs.end()) {
    return i->second;
  }
  cDigitalInputs[name] = new DigitalInput();
  return cDigitalInputs[name];
}

DigitalInput* InputCommands::findDigitalInput(const std::string& name) {
  std::map<std::string, DigitalInput*>::iterator i = cDigitalInputs.find(name);
  return i != cDigitalInputs.end() ? i->second : NULL;
}

DOMNodeWrapper* InputCommands::findConfiguration(std::vector<DOMNodeWrapper*> node, const std::string& name) {
  for (unsigned int i = 0; i < node.size(); i++) {
    for (int j = 0; j < node[i]->getChildCount(); j++) {
      DOMNodeWrapper *mNode = node[i]->getChild(j);
      std::string mValueAsString = mNode->getNodeName();
      if (mValueAsString == "DigitalInput") {
        std::string mName = mNode->getAttribute("name");
        if (mName == name) {
          return mNode;
        }
      }
    }
  }
  return NULL;
}

void InputCommands::loadConfiguration(DOMNodeWrapper* node, std::vector<std::string> configurationFiles, IResources* resources) {
  std::vector<DOMNodeWrapper*> mConfigurationNodes;
  for (unsigned int i = 0; i < configurationFiles.size(); i++) {
    if (!System::fileExists(configurationFiles[i])) {
      DOMNodeWriter* mNode = new DOMNodeWriter("InputConfiguration");
      std::cout << "Creating configuration file: " << configurationFiles[i] << std::endl;
      mNode->save(configurationFiles[i]);
    }
    mConfigurationNodes.push_back(new DOMNodeWrapper(configurationFiles[i]));
  }
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "DigitalInput") {
      std::string mName = mNode->getAttribute("name");
      DigitalInput* mDigitalInput = getDigitalInput(mName);
      DOMNodeWrapper* mConfigurationNode = findConfiguration(mConfigurationNodes, mName);
      if (mConfigurationNode == NULL) {
        mConfigurationNode = mNode;
      }
      mDigitalInput->setup(mNode, resources);
      mDigitalInput->configure(mConfigurationNode);
    } else {
      // TODO: Throw
    }
  }
}

void InputCommands::save(DOMNodeWriter*) {
}

bool* InputCommands::registerDigitalInput(const std::string& name) {
  DigitalInput* mDigitalInput = getDigitalInput(name);
  return mDigitalInput->getDigitalInput();
}

bool InputCommands::input(SDL_Event& event) {
  for (std::map<std::string, DigitalInput*>::iterator i = cDigitalInputs.begin(); i != cDigitalInputs.end(); i++) {
    if (i->second->input(event)) {
      return true;
    }
  }
  return false;
}

std::vector<std::string> InputCommands::getInputNames() {
  std::vector<std::string> mInputNames;
  for (std::map<std::string, DigitalInput*>::iterator i = cDigitalInputs.begin(); i != cDigitalInputs.end(); i++) {
    mInputNames.push_back(i->first);
  }
  return mInputNames;
}

