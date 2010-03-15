#ifndef I_SOUND_SUPPORT_LISTENER_H
#define I_SOUND_SUPPORT_LISTENER_H

#include "ISound.h"

class ISoundSupportListener {
  public:
  virtual void soundAdded(ISound*) = 0;
  virtual void soundRemoved(ISound*) = 0;
};

#endif
