#include "LookAndFeel.h"

IFont* LookAndFeel::cDefaultFont = NULL;
float LookAndFeel::cDefaultFontSize = 1.0f;

void LookAndFeel::setDefaultFont(IFont* font, float size) {
  cDefaultFont = font;
  cDefaultFontSize = size;
}

IFont* LookAndFeel::getDefaultFont() {
  return cDefaultFont;
}

float LookAndFeel::getDefaultFontSize() {
  return cDefaultFontSize;
}



