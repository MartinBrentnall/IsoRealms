#ifndef SDL_MIXER_SOUND_H
#define SDL_MIXER_SOUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "../../../Global/System.h"

#include "../ISound.h"

class SDLMixerSound:public ISound {
  private:
  std::string cName;
  Mix_Chunk* cSoundData;

  public:
  SDLMixerSound(const std::string&);

  /*********************\
   * Implements ISound *
  \*********************/
  std::string& getName();
  void play();
};

#endif
