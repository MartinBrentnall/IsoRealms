#include "BackgroundChanger.h"

BackgroundChanger::BackgroundChanger() {
  cProgressBackgroundColour = 1.0f;
  cPreviousBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
  cTargetBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
}

void BackgroundChanger::update(int ticks) {
  if (cProgressBackgroundColour < 1.0f) {
    cProgressBackgroundColour += 0.0025f * ticks;
    if (cProgressBackgroundColour > 1.0f) {
      cProgressBackgroundColour = 1.0f;
    }
    float mRed   = cPreviousBackgroundColour->getRed()   + (cTargetBackgroundColour->getRed()   - cPreviousBackgroundColour->getRed())   * cProgressBackgroundColour;
    float mGreen = cPreviousBackgroundColour->getGreen() + (cTargetBackgroundColour->getGreen() - cPreviousBackgroundColour->getGreen()) * cProgressBackgroundColour;
    float mBlue  = cPreviousBackgroundColour->getBlue()  + (cTargetBackgroundColour->getBlue()  - cPreviousBackgroundColour->getBlue())  * cProgressBackgroundColour;
    float mFogColour[4] = {mRed, mGreen, mBlue, 0.0f};
    glFogfv(GL_FOG_COLOR, mFogColour);
    glClearColor(mRed, mGreen, mBlue, 0.0f);
  }
}

