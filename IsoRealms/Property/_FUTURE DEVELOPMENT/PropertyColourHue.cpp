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
#include "PropertyColourHue.h"

namespace IsoRealms {
  PropertyColourHue::PropertyColourHue(const std::string& name, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) :
            PropertySlider(name, valueFunction, 0.0f, 1.0f, confirmationCallback),
            cAlpha(alpha),
            cSaturation(saturation),
            cLightness(lightness) {
  }
  
  void PropertyColourHue::renderSlider() const {
    float mY = getY(0.0f);
    float mWidth = getWidth();
    float mHighlightHeight = getSelectionHighlightHeight() * 0.6f;
    
    float mRed;
    float mGreen;
    float mBlue;
    Utils::calculateColour(0.0f, *cSaturation, *cLightness, mRed, mGreen, mBlue);
    glBindTexture(GL_TEXTURE_2D, 0);
    float mDark = std::min({mRed, mGreen, mBlue});
    float mLight = std::max({mRed, mGreen, mBlue});
    float mMid1 = (mWidth / 6) * 1;
    float mMid2 = (mWidth / 6) * 2;
    float mMid3 = (mWidth / 6) * 3;
    float mMid4 = (mWidth / 6) * 4;
    float mMid5 = (mWidth / 6) * 5;
    glEnable(GL_BLEND);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(mLight, mDark, mDark, *cAlpha);
    glVertex2f(0.0f, mY + mHighlightHeight * 0.5f);
    glVertex2f(0.0f, mY - mHighlightHeight * 0.5f);
    glColor4f(mLight, mLight, mDark, *cAlpha);
    glVertex2f(mMid1, mY + mHighlightHeight * 0.5f);
    glVertex2f(mMid1, mY - mHighlightHeight * 0.5f);
    glColor4f(mDark, mLight, mDark, *cAlpha);
    glVertex2f(mMid2, mY + mHighlightHeight * 0.5f);
    glVertex2f(mMid2, mY - mHighlightHeight * 0.5f);
    glColor4f(mDark, mLight, mLight, *cAlpha);
    glVertex2f(mMid3, mY + mHighlightHeight * 0.5f);
    glVertex2f(mMid3, mY - mHighlightHeight * 0.5f);
    glColor4f(mDark, mDark, mLight, *cAlpha);
    glVertex2f(mMid4, mY + mHighlightHeight * 0.5f);
    glVertex2f(mMid4, mY - mHighlightHeight * 0.5f);
    glColor4f(mLight, mDark, mLight, *cAlpha);
    glVertex2f(mMid5, mY + mHighlightHeight * 0.5f);
    glVertex2f(mMid5, mY - mHighlightHeight * 0.5f);
    glColor4f(mLight, mDark, mDark, *cAlpha);
    glVertex2f(mWidth, mY + mHighlightHeight * 0.5f);
    glVertex2f(mWidth, mY - mHighlightHeight * 0.5f);
    glEnd();
    Utils::renderCurve(mWidth, mY, mHighlightHeight * 0.5f, 0.5f, 1.0f);
    Utils::renderCurve(0.0f,   mY, mHighlightHeight * 0.5f, 0.0f, 0.5f);
  }
}
