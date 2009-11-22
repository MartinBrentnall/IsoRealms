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
#include "SoundSet.h"

SoundSet::SoundSet() {
  int audio_rate = 44100;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 1024;
  string cSoundDir = "C64 Sound Scheme/";
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
    fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
    exit(1);
  }
  Mix_AllocateChannels(32);
  loadSound(SWITCH_SQUARE_NONE, cSoundDir + "Square None.ogg");
  loadSound(SWITCH_SQUARE_LEFT, cSoundDir + "Square Left.ogg");
  loadSound(SWITCH_SQUARE_RIGHT, cSoundDir + "Square Right.ogg");
  loadSound(SWITCH_SQUARE_BOTH, cSoundDir + "Square Both.ogg");
  loadSound(SWITCH_CIRCLE_NONE, cSoundDir + "Circle None.ogg");
  loadSound(SWITCH_CIRCLE_LEFT, cSoundDir + "Circle Left.ogg");
  loadSound(SWITCH_CIRCLE_RIGHT, cSoundDir + "Circle Right.ogg");
  loadSound(SWITCH_CIRCLE_BOTH, cSoundDir + "Circle Both.ogg");
  loadSound(SWITCH_DIAMOND_NONE, cSoundDir + "Diamond None.ogg");
  loadSound(SWITCH_DIAMOND_LEFT, cSoundDir + "Diamond Left.ogg");
  loadSound(SWITCH_DIAMOND_RIGHT, cSoundDir + "Diamond Right.ogg");
  loadSound(SWITCH_DIAMOND_BOTH, cSoundDir + "Diamond Both.ogg");
  loadSound(JEWEL_COLLECT, cSoundDir + "Jewel.ogg");
  loadSound(LIFT_CLICK_1, cSoundDir + "Lift Click 1.ogg");
  loadSound(LIFT_CLICK_2, cSoundDir + "Lift Click 2.ogg");
  loadSound(LIFT_CLICK_3, cSoundDir + "Lift Click 3.ogg");
  loadSound(TRAMPOLINE_BOUNCE, cSoundDir + "Trampoline.ogg");
  loadSound(IMPACT_SOFT, cSoundDir + "Impact Soft.ogg");
  loadSound(IMPACT_MEDIUM, cSoundDir + "Impact Medium.ogg");
  loadSound(IMPACT_HARD, cSoundDir + "Impact Hard.ogg");
  loadSound(RESPAWN_HARD, cSoundDir + "Respawn Hard.ogg");

//  loadSound(MUSIC, "Music/SpindizzyMIDI.ogg");
}

SoundSet::~SoundSet() {
  
}

void SoundSet::loadSound(int type, string file) {
  cSounds[type] = Mix_LoadWAV(file.c_str());
  if (cSounds[type] == NULL) {
    cout << "WARNING: Unable to load file: " << Mix_GetError() << endl;
  }
}

void SoundSet::play(int sound) {
  if (Mix_PlayChannel(-1, cSounds[sound], 0) == -1) {
    cout << "WARNING: Unable to play file: " << Mix_GetError() << endl;
  }
}
