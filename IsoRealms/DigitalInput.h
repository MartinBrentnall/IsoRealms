#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <set>
#include <vector>
#include <SDL/SDL.h>

#include "DOMNodeWrapper.h"
#include "IResources.h"
#include "Script.h"
#include "SDLKeyMap.h"

class DigitalInput {
  private:
  static SDLKeyMap cSDLKeyMap;
  bool* cInput;
  std::set<SDLKey> cActiveKeys;
  std::vector<SDLKey> cKeys;
  Script* cActivatedScript;
    
  bool keyDown(SDLKey& key);
  bool keyUp(SDLKey& key);
  bool isMapped(SDLKey& key);
  SDLKey getSDLKey(const std::string&);

  
  public:
  DigitalInput();
  void setup(DOMNodeWrapper*, IResources*);
  void configure(DOMNodeWrapper*);
  bool input(SDL_Event&);
  bool* getDigitalInput();
};

#endif
