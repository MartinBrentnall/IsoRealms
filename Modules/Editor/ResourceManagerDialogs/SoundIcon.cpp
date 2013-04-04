#include "SoundIcon.h"

SoundIcon::SoundIcon(IResourceBrowser<ISound>* browser, ISound* sound) : Icon<ISound>(browser, sound) {
}

void SoundIcon::renderIcon() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  glColor3f(1.0f, 1.0f, 1.0f);
  mFont->print(mLeft + 0.01f, mBottom + 0.01f, mFontSize, 0, getResourceLocation().c_str());
}

void SoundIcon::iconSelected() {
  ISound* mSound = getResource();
  mSound->play();
}

float SoundIcon::getWidth() {
  return 0.66f;
}

float SoundIcon::getHeight() {
  return 0.05f;
}
