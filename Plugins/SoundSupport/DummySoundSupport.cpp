#include "DummySoundSupport.h"

std::vector<ISound*> DummySoundSupport::getSounds() {
  std::vector<ISound*> mEmptyVector;
  return mEmptyVector;
}

void DummySoundSupport::addSoundSupportListener(ISoundSupportListener*) {
  // Nothing to do.
}

void DummySoundSupport::removeSoundSupportListener(ISoundSupportListener*) {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new DummySoundSupport();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}


