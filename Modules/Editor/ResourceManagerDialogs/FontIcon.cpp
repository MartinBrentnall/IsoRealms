#include "FontIcon.h"

FontIcon::FontIcon(IResourceBrowser<IFont>* browser, IFont* font) : Icon<IFont>(browser, font) {
}

void FontIcon::renderIcon() {
  IFont* mFont = getResource();
  float mLeft = getLeft();
  float mBottom = getBottom();
  glColor3f(1.0f, 1.0f, 1.0f);
  float mFontSize = LookAndFeel::getDefaultFontSize();
  mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, 0, getResourceLocation().c_str());
}

float FontIcon::getWidth() {
  return 0.4f;
}

float FontIcon::getHeight() {
  return 0.1f;
}
