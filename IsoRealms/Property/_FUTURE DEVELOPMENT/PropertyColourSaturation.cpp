/*
 * Copyright 2023 Martin Brentnall
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
#include "PropertyColourSaturation.h"

namespace IsoRealms {
  PropertyColourSaturation::PropertyColourSaturation(const std::string& name, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) :
            PropertySlider(name, valueFunction, 0.0f, 1.0f, confirmationCallback),
            cAlpha(alpha),
            cHue(hue),
            cLightness(lightness) {
  }
  
  void PropertyColourSaturation::renderSlider() const {
    float mRed;
    float mGreen;
    float mBlue;
    Utils::calculateColour(*cHue, 0.0f, *cLightness, mRed, mGreen, mBlue);
    float mY = getY(0.0f);
    float mWidth = getWidth();
    float mHighlightHeight = getSelectionHighlightHeight() * 0.6f;
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor4f(*cLightness, *cLightness, *cLightness, *cAlpha);
    glVertex2f(mWidth, mY - mHighlightHeight * 0.5f);
    glVertex2f(mWidth, mY + mHighlightHeight * 0.5f);
    glColor4f(mRed, mGreen, mBlue, *cAlpha);
    glVertex2f(0.0f,   mY + mHighlightHeight * 0.5f);
    glVertex2f(0.0f,   mY - mHighlightHeight * 0.5f);
    glEnd();
    glColor4f(*cLightness, *cLightness, *cLightness, *cAlpha);
    Utils::renderCurve(mWidth, mY, mHighlightHeight * 0.5f, 0.5f, 1.0f);
    glColor4f(mRed, mGreen, mBlue, *cAlpha);
    Utils::renderCurve(0.0f,   mY, mHighlightHeight * 0.5f, 0.0f, 0.5f);
  }
}

