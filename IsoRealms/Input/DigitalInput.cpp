/*
 * Copyright 2015 Martin Brentnall
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
#include "DigitalInput.h"

SDLKeyMap DigitalInput::cSDLKeyMap;

DigitalInput::DigitalInput() {
  cActivatedScript = nullptr;
  cInput = new bool;
  *cInput = false;
}

void DigitalInput::setup(DOMNodeWrapper* node, IResources* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "OnStart") {
      cActivatedScript = resources->getScriptCall(mNode);
    }
  }
}

void DigitalInput::configure(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Mapping") {
      std::string mType = mNode->getAttribute("type");
      std::string mValue = mNode->getAttribute("value");
      if (mType == "keyDown") {
        SDLKey mKey = cSDLKeyMap.getKey(mValue);
        cKeys.push_back(mKey);
      } else {
        // TODO: Throw something
      }
    }
  }
}

void DigitalInput::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cKeys.size(); i++) {
    DOMNodeWriter* mMappingNode = node->addBranch("Mapping");
    mMappingNode->addAttribute("type", "keyDown");
    mMappingNode->addAttribute("value", cSDLKeyMap.getName(cKeys[i]));
  }
  if (cActivatedScript != nullptr) {
    DOMNodeWriter* mStartScriptNode = node->addBranch("OnStart");
    cActivatedScript->save(mStartScriptNode, resourceLocator);
  }
}

bool DigitalInput::isMapped(SDLKey& key) {
  for (unsigned int i = 0; i < cKeys.size(); i++) {
    if (cKeys[i] == key) {
      return true;
    }
  }
  return false;
}

bool DigitalInput::keyDown(SDLKey& key) {
  if (isMapped(key)) {
    if (cActiveKeys.empty()) {
      *cInput = true;
      if (cActivatedScript != nullptr) {
        cActivatedScript->execute();
      }
    }
    cActiveKeys.insert(key);
    return true;
  }
  return false;
}  

bool DigitalInput::keyUp(SDLKey& key) {
  if (isMapped(key)) {
    cActiveKeys.erase(key);
    if (cActiveKeys.empty()) {
      *cInput = false;
    }
    return true;
  }
  return false;
}

bool DigitalInput::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
    
    case SDL_KEYUP: {
      return keyUp(event.key.keysym.sym);
    }
  }
  return false;
}

void DigitalInput::trigger(bool state) {
  if (!(*cInput) && state && cActivatedScript != nullptr) {
    cActivatedScript->execute();
  }
  *cInput = state;
}

bool* DigitalInput::getDigitalInput() {
  return cInput;
}
