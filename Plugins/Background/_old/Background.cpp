/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Background.h"

Background::Background() {
  Colour mDefaultBackground(0.0, 0.0, 0.0);
  set(mDefaultBackground);
}

Background::Background(Colour& colour) {
  set(colour);
}

void Background::set(Colour& colour) {
  cPreviousColour = cCurrentColour;
  cCurrentColour = colour;
  cProgress = 0.0;
}

void Background::update(int ticks) {
  if (cProgress < 1.0) {
    cProgress += TRANSITION_SPEED * ticks;
    if (cProgress > 1.0) {
      cProgress = 1.0;
    }
    float mRed = sine(cPreviousColour.getRed(), cCurrentColour.getRed(), cProgress);
    float mGreen = sine(cPreviousColour.getGreen(), cCurrentColour.getGreen(), cProgress);
    float mBlue = sine(cPreviousColour.getBlue(), cCurrentColour.getBlue(), cProgress);
    glClearColor(mRed, mGreen, mBlue, 1.0);
    float mColour[4] = {mRed, mGreen, mBlue, 0.0};
    glFogfv(GL_FOG_COLOR, mColour);
  }
}
