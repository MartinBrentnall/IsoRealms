#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <set>
#include <vector>
#include <SDL/SDL.h>

#include "CommandDirectory.h"
#include "DOMNodeWrapper.h"
#include "Script.h"

class DigitalInput {
  private:
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
  void configure(DOMNodeWrapper*, CommandDirectory*);
  bool input(SDL_Event&);
  bool* getDigitalInput();
};

#endif
