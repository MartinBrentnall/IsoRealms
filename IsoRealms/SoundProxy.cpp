#include "SoundProxy.h"

SoundProxy::SoundProxy() {
  cSound = NULL;
}
 
void SoundProxy::setImplementation(ISound* sound) {
  cSound = sound;
}

std::string SoundProxy::getName() {
  return cSound != NULL ? cSound->getName() : NULL;
}

void SoundProxy::play() {
  if (cSound != NULL) {
    cSound->play();
  }
}

