#ifndef SOUND_PROXY_H
#define SOUND_PROXY_H

#include "ISound.h"

class SoundProxy:public ISound {
  private:
  ISound* cSound;
  
  public:
  SoundProxy();
  
  void setImplementation(ISound*);
  
  /*********************\
   * Implements ISound *
  \*********************/
  std::string getName();
  void play();
};

#endif
