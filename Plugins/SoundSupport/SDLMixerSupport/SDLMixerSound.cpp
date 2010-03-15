#include "SDLMixerSound.h"

SDLMixerSound::SDLMixerSound(const std::string& filename) {
  cName = filename;
  std::string mResource = System::getResource("Sounds/" + cName);
  cSoundData = Mix_LoadWAV(mResource.c_str());
  if (cSoundData == NULL) {
    std::cout << "WARNING: Unable to load file: " << Mix_GetError() << std::endl;
  }
}

std::string& SDLMixerSound::getName() {
  return cName;
}

void SDLMixerSound::play() {
  if (Mix_PlayChannel(-1, cSoundData, 0) == -1) {
    std::cout << "WARNING: Unable to play file: " << Mix_GetError() << std::endl;
  }
}
