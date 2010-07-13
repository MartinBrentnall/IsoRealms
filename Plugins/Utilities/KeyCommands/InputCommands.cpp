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
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
}

SDLKey InputCommands::getSDLKey(const std::string& name) {
  if (name == "1") {return SDLK_1;}
  if (name == "2") {return SDLK_2;}
  if (name == "3") {return SDLK_3;}
  if (name == "4") {return SDLK_4;}
  if (name == "5") {return SDLK_5;}
  if (name == "6") {return SDLK_6;}
  if (name == "7") {return SDLK_7;}
  if (name == "8") {return SDLK_8;}
  if (name == "9") {return SDLK_9;}
  if (name == "0") {return SDLK_0;}
  if (name == "[") {return SDLK_LEFTBRACKET;}
  if (name == "]") {return SDLK_RIGHTBRACKET;}
  if (name == "a") {return SDLK_a;}
  if (name == "b") {return SDLK_b;}
  if (name == "c") {return SDLK_c;}
  if (name == "d") {return SDLK_d;}
  if (name == "e") {return SDLK_e;}
  if (name == "f") {return SDLK_f;}
  if (name == "g") {return SDLK_g;}
  if (name == "h") {return SDLK_h;}
  if (name == "i") {return SDLK_i;}
  if (name == "j") {return SDLK_j;}
  if (name == "k") {return SDLK_k;}
  if (name == "l") {return SDLK_l;}
  if (name == "m") {return SDLK_m;}
  if (name == "n") {return SDLK_n;}
  if (name == "o") {return SDLK_o;}
  if (name == "p") {return SDLK_p;}
  if (name == "q") {return SDLK_q;}
  if (name == "r") {return SDLK_r;}
  if (name == "s") {return SDLK_s;}
  if (name == "t") {return SDLK_t;}
  if (name == "u") {return SDLK_u;}
  if (name == "v") {return SDLK_v;}
  if (name == "w") {return SDLK_w;}
  if (name == "x") {return SDLK_x;}
  if (name == "y") {return SDLK_y;}
  if (name == "z") {return SDLK_z;}
  std::cout << "Unrecognised key name: \"" << name << "\"" << std::endl;
  exit(1);
}

std::vector<IUserCommand*> InputCommands::getCommands(DOMNodeWrapper* node) {
  std::vector<IUserCommand*> mCommands;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      std::string mCommandName = mNode->getStringValue();
      mCommands.push_back(cCommandRegistry->getCommand(mCommandName));
    } else {
      // TODO: Throw
    }
  }
  return mCommands;
}

void InputCommands::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Input") {
      std::string mType = mNode->getAttribute("type");
      std::string mValue = mNode->getAttribute("value");
      std::vector<IUserCommand*> mCommands = getCommands(mNode);
      if (mType == "key") {
        cKeyCommands[getSDLKey(mValue)] = mCommands;
      } else {
        // TODO: Throw
        std::cout << "Unknown input type: \"" << mType << "\"" << std::endl;
        exit(1);
      }
    } else {
      // TODO: Throw
    }
  }
}

void InputCommands::save(DOMNodeWriter*) {
}

std::vector<PlugSocket*> InputCommands::getPlugSockets() {
  return cSockets;
}

void InputCommands::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    if (assignPlugin(plugin, &cCommandRegistry, *socket)) {
      cKeyCommands.clear();
    }
  }
}

IPlugin* InputCommands::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  // TODO: Throw something
  return NULL;
}

std::vector<IInteractiveElement*> InputCommands::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  mInteractiveElements.push_back(this);
  return mInteractiveElements;
}

bool InputCommands::keyDown(SDLKey& key) {
  std::map<SDLKey, std::vector<IUserCommand*> >::iterator i = cKeyCommands.find(key);
  if (i != cKeyCommands.end()) {
    for (unsigned int j = 0; j < i->second.size(); j++) {
      i->second[j]->execute();
    }
    return true;
  }
  return false;
}  

bool InputCommands::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

extern "C" IPlugin* create() {
  return new InputCommands();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
