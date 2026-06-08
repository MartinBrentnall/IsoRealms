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
#pragma once

#include <limits>
#include <functional>
#include <string>

#include <GL/glew.h>

#include "PropertySlider.h"

namespace IsoRealms {
  class PropertyColourChannel : public PropertySlider {
    public:
    static float MIN_CHANNEL_VALUE;
    static float MAX_CHANNEL_VALUE;
    
    PropertyColourChannel(IPropertyMaker& owner, const std::string& key, const Metadata& metadata, const PropertyData& data, IResourceAccessManager& resourceAccessManager, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback);
      
    /*****************************\
     * Implements PropertySlider *
    \*****************************/
    void renderSlider(float scale, float width, float y, float x, float aspectRatio) const override;

    private:
    float* cMinRed;
    float* cMinGreen;
    float* cMinBlue;
    float* cMinAlpha;
    float* cMaxRed;
    float* cMaxGreen;
    float* cMaxBlue;
    float* cMaxAlpha;
  };
}
