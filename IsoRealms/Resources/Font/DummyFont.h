#ifndef DUMMY_FONT_H
#define DUMMY_FONT_H

#include "IFont.h"

class DummyFont:public IFont {
  public:
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void print(float, float, float, int, const char*, ...);
  float getWidth(float, const char*, ...);
};

#endif
