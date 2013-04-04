#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <set>
#include <vector>
#include <SDL/SDL.h>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResources.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/IScript.h>

#include "SDLKeyMap.h"

class DigitalInput {
  private:
  static SDLKeyMap cSDLKeyMap;
  bool* cInput;
  std::set<SDLKey> cActiveKeys;
  std::vector<SDLKey> cKeys;
  IScript* cActivatedScript;
    
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
  bool* getDigitalInput();
};

#endif
