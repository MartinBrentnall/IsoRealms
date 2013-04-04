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
#include "SDLMixerSound.h"

SDLMixerSound::SDLMixerSound() {
}

SDLMixerSound::SDLMixerSound(const std::string& filename) {
  cName = filename;
  loadData();
}

void SDLMixerSound::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cName = node->getStringValue();
  loadData();
}

void SDLMixerSound::loadData() {
  std::string mResource = System::getProgramResource("Data/Sounds/" + cName);
  cSoundData = Mix_LoadWAV(mResource.c_str());
  if (cSoundData == NULL) {
    std::cout << "WARNING: Unable to load file: " << Mix_GetError() << std::endl;
  }
}

std::string SDLMixerSound::getName() {
  return cName;
}

void SDLMixerSound::play() {
  if (Mix_PlayChannel(-1, cSoundData, 0) == -1) {
    std::cout << "WARNING: Unable to play file: " << Mix_GetError() << std::endl;
  }
}
