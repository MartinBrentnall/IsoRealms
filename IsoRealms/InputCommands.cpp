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

DigitalInput* InputCommands::getDigitalInput(const std::string& name) {
  std::map<std::string, DigitalInput*>::iterator i = cDigitalInputs.find(name);
  if (i != cDigitalInputs.end()) {
    return i->second;
  }
  cDigitalInputs[name] = new DigitalInput();
  return cDigitalInputs[name];
}

void InputCommands::loadConfiguration(DOMNodeWrapper* node, DOMNodeWrapper* globalConfiguration, DOMNodeWrapper* projectConfiguration, CommandDirectory* commandRegistry) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "DigitalInput") {
      std::string mName = mNode->getAttribute("name");
      DigitalInput* mDigitalInput = getDigitalInput(mName);
      mDigitalInput->configure(mNode, globalConfiguration, projectConfiguration, commandRegistry);
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
