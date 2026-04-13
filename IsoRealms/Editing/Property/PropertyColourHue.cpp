/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "PropertyColourHue.h"

#include "IsoRealms/Utils.h"

namespace IsoRealms {
  PropertyColourHue::PropertyColourHue(IPropertyMaker& owner, const PropertyData& data, IResourceAccessManager& resourceAccessManager, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) :
            PropertySlider(owner, data, resourceAccessManager, valueFunction, 0.0f, 1.0f, confirmationCallback, nullptr),
            cAlpha(alpha),
            cSaturation(saturation),
            cLightness(lightness) {
  }
  
  void PropertyColourHue::renderSlider(float scale, float width, float y, float x, float aspectRatio) const {
    float mRed;
    float mGreen;
    float mBlue;
    Utils::calculateColour(0.0f, *cSaturation, *cLightness, mRed, mGreen, mBlue);
    glBindTexture(GL_TEXTURE_2D, 0);
    float mDark = std::min({mRed, mGreen, mBlue});
    float mLight = std::max({mRed, mGreen, mBlue});
    float mMid1 = (width / 6) * 1;
    float mMid2 = (width / 6) * 2;
    float mMid3 = (width / 6) * 3;
    float mMid4 = (width / 6) * 4;
    float mMid5 = (width / 6) * 5;
    glEnable(GL_BLEND);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(mLight, mDark, mDark, *cAlpha);
    glVertex2f(x, y + scale * 1.5f);
    glVertex2f(x, y + scale * 0.5f);
    glColor4f(mLight, mLight, mDark, *cAlpha);
    glVertex2f(x + mMid1, y + scale * 1.5f);
    glVertex2f(x + mMid1, y + scale * 0.5f);
    glColor4f(mDark, mLight, mDark, *cAlpha);
    glVertex2f(x + mMid2, y + scale * 1.5f);
    glVertex2f(x + mMid2, y + scale * 0.5f);
    glColor4f(mDark, mLight, mLight, *cAlpha);
    glVertex2f(x + mMid3, y + scale * 1.5f);
    glVertex2f(x + mMid3, y + scale * 0.5f);
    glColor4f(mDark, mDark, mLight, *cAlpha);
    glVertex2f(x + mMid4, y + scale * 1.5f);
    glVertex2f(x + mMid4, y + scale * 0.5f);
    glColor4f(mLight, mDark, mLight, *cAlpha);
    glVertex2f(x + mMid5, y + scale * 1.5f);
    glVertex2f(x + mMid5, y + scale * 0.5f);
    glColor4f(mLight, mDark, mDark, *cAlpha);
    glVertex2f(x + width, y + scale * 1.5f);
    glVertex2f(x + width, y + scale * 0.5f);
    glEnd();
    Utils::renderCurve(x + width, y + scale, scale * 0.5f, 0.5f, 1.0f);
    Utils::renderCurve(x,         y + scale, scale * 0.5f, 0.0f, 0.5f);
  }
}
