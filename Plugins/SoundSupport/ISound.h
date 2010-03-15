#ifndef I_SOUND_H
#define I_SOUND_H

#include <string>

class ISound {
  public:
  virtual std::string& getName() = 0;
  virtual void play() = 0;
};

#endif
