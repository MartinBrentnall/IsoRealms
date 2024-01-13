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
#include "PropertyColourLightness.h"

namespace IsoRealms {
  PropertyColourLightness::PropertyColourLightness(const std::string& name, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) :
            PropertySlider(name, valueFunction, 0.0f, 1.0f, confirmationCallback),
            cAlpha(alpha),
            cHue(hue),
            cSaturation(saturation) {
  }
  
  void PropertyColourLightness::renderSlider() const {
    float mY = getY(0.0f);
    float mWidth = getWidth();
    float mHighlightHeight = getSelectionHighlightHeight() * 0.6f;
    float mRed;
    float mGreen;
    float mBlue;
    Utils::calculateColour(*cHue, *cSaturation, 1.0f, mRed, mGreen, mBlue);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor4f(mRed, mGreen, mBlue, *cAlpha);
    glVertex2f(mWidth, mY - mHighlightHeight * 0.5f);
    glVertex2f(mWidth, mY + mHighlightHeight * 0.5f);
    glColor4f(0.0f, 0.0f, 0.0f, *cAlpha);
    glVertex2f(0.0f,   mY + mHighlightHeight * 0.5f);
    glVertex2f(0.0f,   mY - mHighlightHeight * 0.5f);
    glEnd();
    glColor4f(mRed, mGreen, mBlue, *cAlpha);
    Utils::renderCurve(mWidth, mY, mHighlightHeight * 0.5f, 0.5f, 1.0f);
    glColor4f(0.0f, 0.0f, 0.0f, *cAlpha);
    Utils::renderCurve(0.0f,   mY, mHighlightHeight * 0.5f, 0.0f, 0.5f);
  }
}

