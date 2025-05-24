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
#include "PropertyColourChannel.h"

namespace IsoRealms {
  float PropertyColourChannel::MIN_CHANNEL_VALUE = 0.0f;
  float PropertyColourChannel::MAX_CHANNEL_VALUE = 1.0f;
  
  PropertyColourChannel::PropertyColourChannel(const std::string& name, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) :
            PropertySlider(name, valueFunction, 0.0f, 1.0f, confirmationCallback, nullptr),
            cMinRed(minRed),
            cMinGreen(minGreen),
            cMinBlue(minBlue),
            cMinAlpha(minAlpha),
            cMaxRed(maxRed),
            cMaxGreen(maxGreen),
            cMaxBlue(maxBlue),
            cMaxAlpha(maxAlpha) {
  }
  
  void PropertyColourChannel::renderSlider(float scale, float width, float y, float x, float aspectRatio) const {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor4f(*cMaxRed, *cMaxGreen, *cMaxBlue, *cMaxAlpha);
    glVertex2f(x + width, y + scale * 0.5f);
    glVertex2f(x + width, y + scale * 1.5f);
    glColor4f(*cMinRed, *cMinGreen, *cMinBlue, *cMinAlpha);
    glVertex2f(x,         y + scale * 1.5f);
    glVertex2f(x,         y + scale * 0.5f);
    glEnd();
    glColor4f(*cMaxRed, *cMaxGreen, *cMaxBlue, *cMaxAlpha);
    Utils::renderCurve(x + width, y + scale, scale * 0.5f, 0.5f, 1.0f);
    glColor4f(*cMinRed, *cMinGreen, *cMinBlue, *cMinAlpha);
    Utils::renderCurve(x,         y + scale, scale * 0.5f, 0.0f, 0.5f);
  }
}
