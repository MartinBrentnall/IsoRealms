#ifndef DUMMY_SOUND_H
#define DUMMY_SOUND_H

#include "ISound.h"

class DummySound:public ISound {
  public:
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void play();
};

#endif
