/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "SDLKeyMap.h"

SDLKeyMap::SDLKeyMap() {
  cKeysByName["1"]            = SDLK_1;
  cKeysByName["2"]            = SDLK_2;
  cKeysByName["3"]            = SDLK_3;
  cKeysByName["4"]            = SDLK_4;
  cKeysByName["5"]            = SDLK_5;
  cKeysByName["6"]            = SDLK_6;
  cKeysByName["7"]            = SDLK_7;
  cKeysByName["8"]            = SDLK_8;
  cKeysByName["9"]            = SDLK_9;
  cKeysByName["0"]            = SDLK_0;
  cKeysByName["["]            = SDLK_LEFTBRACKET;
  cKeysByName["]"]            = SDLK_RIGHTBRACKET;
  cKeysByName["a"]            = SDLK_a;
  cKeysByName["b"]            = SDLK_b;
  cKeysByName["c"]            = SDLK_c;
  cKeysByName["d"]            = SDLK_d;
  cKeysByName["e"]            = SDLK_e;
  cKeysByName["f"]            = SDLK_f;
  cKeysByName["g"]            = SDLK_g;
  cKeysByName["h"]            = SDLK_h;
  cKeysByName["i"]            = SDLK_i;
  cKeysByName["j"]            = SDLK_j;
  cKeysByName["k"]            = SDLK_k;
  cKeysByName["l"]            = SDLK_l;
  cKeysByName["m"]            = SDLK_m;
  cKeysByName["n"]            = SDLK_n;
  cKeysByName["o"]            = SDLK_o;
  cKeysByName["p"]            = SDLK_p;
  cKeysByName["q"]            = SDLK_q;
  cKeysByName["r"]            = SDLK_r;
  cKeysByName["s"]            = SDLK_s;
  cKeysByName["t"]            = SDLK_t;
  cKeysByName["u"]            = SDLK_u;
  cKeysByName["v"]            = SDLK_v;
  cKeysByName["w"]            = SDLK_w;
  cKeysByName["x"]            = SDLK_x;
  cKeysByName["y"]            = SDLK_y;
  cKeysByName["z"]            = SDLK_z;
  cKeysByName["F1"]           = SDLK_F1;
  cKeysByName["F2"]           = SDLK_F2;
  cKeysByName["F3"]           = SDLK_F3;
  cKeysByName["F4"]           = SDLK_F4;
  cKeysByName["F5"]           = SDLK_F5;
  cKeysByName["F6"]           = SDLK_F6;
  cKeysByName["F7"]           = SDLK_F7;
  cKeysByName["F8"]           = SDLK_F8;
  cKeysByName["F9"]           = SDLK_F9;
  cKeysByName["F10"]          = SDLK_F10;
  cKeysByName["F11"]          = SDLK_F11;
  cKeysByName["F12"]          = SDLK_F12;
  cKeysByName["F13"]          = SDLK_F13;
  cKeysByName["F14"]          = SDLK_F14;
  cKeysByName["F15"]          = SDLK_F15;
  cKeysByName["ArrowUp"]      = SDLK_UP;
  cKeysByName["ArrowLeft"]    = SDLK_LEFT;
  cKeysByName["ArrowRight"]   = SDLK_RIGHT;
  cKeysByName["ArrowDown"]    = SDLK_DOWN;
  cKeysByName["LeftControl"]  = SDLK_LCTRL;
  cKeysByName["RightControl"] = SDLK_RCTRL;
  cKeysByName["Space"]        = SDLK_SPACE;
}

SDLKey SDLKeyMap::getKey(const std::string& name) {
  std::map<std::string, SDLKey>::iterator i = cKeysByName.find(name);
  if (i == cKeysByName.end()) {
    std::cout << "Key \"" << name << "\" not known!" << std::endl;
    exit(1);
  }
  return i->second;
}

std::string SDLKeyMap::getName(SDLKey& key) {
  for (std::map<std::string, SDLKey>::iterator i = cKeysByName.begin(); i != cKeysByName.end(); i++) {
    if (i->second == key) {
      return i->first;
    }
  }
  std::cout << "Key \"" << key << "\" has no name!" << std::endl;
  exit(1);
}
