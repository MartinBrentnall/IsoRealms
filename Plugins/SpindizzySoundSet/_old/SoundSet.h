/*
 * Copyright 2009 Martin Brentnall
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
#ifndef SOUND_SET_H
#define SOUND_SET_H

#include <map>
#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "SpindizzyConstants.h"

using namespace std;

// TODO: Have SoundSchemes like with TextureSets.
class SoundSet:public SpindizzyConstants {
  private:
  /** Allocated sound data. */
  map<int, Mix_Chunk*> cSounds;

  /**
   * Load the sound file of the specified name and allocate it as the specified
   * type.
   */
  void loadSound(int type, string name);

  public:
  /**
   * Construct a new sound set with C64 sound scheme.
   */
  SoundSet();

  /**
   * Unload sound scheme.
   */
  ~SoundSet();

  /**
   * Play the specified sound.
   */
  void play(int sound);
};

#endif
