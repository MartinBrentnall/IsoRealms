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
#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <set>
#include <vector>
#include <SDL/SDL.h>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResources.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Script/IScriptCall.h>

#include "SDLKeyMap.h"

class DigitalInput {
  private:
  static SDLKeyMap cSDLKeyMap;
  bool cTriggered;
  bool* cInput;
  std::set<SDLKey> cActiveKeys;
  std::vector<SDLKey> cKeys;
  IScriptCall* cActivatedScript;
  IScriptCall* cDeactivatedScript;
    
  bool keyDown(SDLKey& key);
  bool keyUp(SDLKey& key);
  bool isMapped(SDLKey& key);
  SDLKey getSDLKey(const std::string&);
  
  public:
  DigitalInput();
  void setup(DOMNodeWrapper*, IResources*);
  void configure(DOMNodeWrapper*);
  void save(DOMNodeWriter*, IResourceLocator*);
  bool input(SDL_Event&);
  void trigger(bool);
  bool* getDigitalInput();
  IScriptCall* getActivatedScript();
  IScriptCall* getDeactivatedScript();
  std::vector<SDLKey> getKeys();
};

#endif
