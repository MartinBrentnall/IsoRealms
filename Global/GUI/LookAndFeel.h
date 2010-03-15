#ifndef LOOK_AND_FEEL_H
#define LOOK_AND_FEEL_H

#include <iostream>

#include "../IFont.h"

class LookAndFeel {
  private:
  static IFont* cDefaultFont;
  static float cDefaultFontSize;

  public:
  static void setDefaultFont(IFont*, float);
  static IFont* getDefaultFont();
  static float getDefaultFontSize();
};

#endif
