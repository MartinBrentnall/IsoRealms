#ifndef I_SOUND_REGISTRY_H
#define I_SOUND_REGISTRY_H

#include "SDLMixerSound.h"

class ISoundRegistry {
  public:
  virtual void addSound(ISound*) = 0;
  virtual void removeSound(ISound*) = 0;
  virtual std::vector<ISound*> getSounds() = 0;
};

#endif
