#ifndef SDL_KEY_MAP_H
#define SDL_KEY_MAP_H

#include <iostream>
#include <map>
#include <SDL/SDL.h>
#include <string>

class SDLKeyMap {
  private:
  std::map<std::string, SDLKey> cKeysByName;

  public:
  SDLKeyMap();
  SDLKey getKey(const std::string&);
  std::string getName(SDLKey&);
};

#endif
