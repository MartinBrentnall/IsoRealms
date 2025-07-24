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
#include "PropertyColourSaturation.h"

#include "IsoRealms/Utils.h"

namespace IsoRealms {
  PropertyColourSaturation::PropertyColourSaturation(PropertyMaker& owner, const PropertyData& data, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) :
            PropertySlider(owner, data, valueFunction, 0.0f, 1.0f, confirmationCallback, nullptr),
            cAlpha(alpha),
            cHue(hue),
            cLightness(lightness) {
  }
  
  void PropertyColourSaturation::renderSlider(float scale, float width, float y, float x, float aspectRatio) const {
    float mRed;
    float mGreen;
    float mBlue;
    Utils::calculateColour(*cHue, 0.0f, *cLightness, mRed, mGreen, mBlue);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor4f(*cLightness, *cLightness, *cLightness, *cAlpha);
    glVertex2f(x + width, y + scale * 0.5f);
    glVertex2f(x + width, y + scale * 1.5f);
    glColor4f(mRed, mGreen, mBlue, *cAlpha);
    glVertex2f(x,         y + scale * 1.5f);
    glVertex2f(x,         y + scale * 0.5f);
    glEnd();
    glColor4f(*cLightness, *cLightness, *cLightness, *cAlpha);
    Utils::renderCurve(x + width, y + scale, scale * 0.5f, 0.5f, 1.0f);
    glColor4f(mRed, mGreen, mBlue, *cAlpha);
    Utils::renderCurve(x,         y + scale, scale * 0.5f, 0.0f, 0.5f);
  }
}

