#ifndef DUMMY_SOUND_SUPPORT_H
#define DUMMY_SOUND_SUPPORT_H

#include "../../Global/DummyPlugin.h"

#include "ISoundSupport.h"

class DummySoundSupport:public ISoundSupport,
                        public DummyPlugin {
  public:
  std::vector<ISound*> getSounds();
  void addSoundSupportListener(ISoundSupportListener*);
  void removeSoundSupportListener(ISoundSupportListener*);
};

#endif
