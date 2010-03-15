#ifndef I_SOUND_SUPPORT_H
#define I_SOUND_SUPPORT_H

#include "../../Global/IPlugin.h"

#include "ISound.h"
#include "ISoundSupportListener.h"

class ISoundSupport:public virtual IPlugin {
  public:
  virtual std::vector<ISound*> getSounds() = 0;
  virtual void addSoundSupportListener(ISoundSupportListener*) = 0;
  virtual void removeSoundSupportListener(ISoundSupportListener*) = 0;
};

#endif
