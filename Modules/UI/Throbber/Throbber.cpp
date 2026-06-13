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
#include "Throbber.h"

namespace IsoRealms::UI {
  Throbber::Throbber(UI& ui, IResourceData& data) :
            cDefDuration(DEFAULT_DURATION),
            cDefRepetitions(DEFAULT_REPETITIONS),
            cDefSpots(DEFAULT_SPOTS),
            cDefSpotSides(DEFAULT_SPOT_SIDES),
            cDefSpotRadius(DEFAULT_SPOT_RADIUS),
            cDefRingRadius(DEFAULT_RING_RADIUS),
            cDefShadowOffset(DEFAULT_SHADOW_OFFSET),
            cDefColour(data, 1.0f, 0.0f, 1.0f) {
    cRuntimeAnimation = 0U;
  }

  void Throbber::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IScreen>(this, "", "Throbbers");
  }

  void Throbber::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Throbber::renderIcon() const {
    return false;
  }

  void Throbber::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeUnsignedInteger("duration",     [this]() {return cDefDuration;},     [this](unsigned int value) {cDefDuration     = value;}, DEFAULT_DURATION);
    owner.createPropertyNativeUnsignedInteger("spots",         [this]() {return cDefSpots;},        [this](unsigned int value) {cDefSpots        = value;}, DEFAULT_SPOTS);
    owner.createPropertyNativeUnsignedInteger("spotSides",    [this]() {return cDefSpotSides;},    [this](unsigned int value) {cDefSpotSides    = value;}, DEFAULT_SPOT_SIDES);
    owner.createPropertyNativeFloat(          "spotRadius",   [this]() {return cDefSpotRadius;},   [this](float        value) {cDefSpotRadius   = value;}, DEFAULT_SPOT_RADIUS);
    owner.createPropertyNativeFloat(          "shadowOffset", [this]() {return cDefShadowOffset;}, [this](float        value) {cDefShadowOffset = value;}, DEFAULT_SHADOW_OFFSET);
    owner.createPropertyTreeSelector(         "colour",       cDefColour);
    owner.createPropertyNativeFloat(          "ringRadius",   [this]() {return cDefRingRadius;},   [this](float        value) {cDefRingRadius   = value;}, DEFAULT_RING_RADIUS);
    owner.createPropertyNativeUnsignedInteger("repetitions",  [this]() {return cDefRepetitions;},  [this](unsigned int value) {cDefRepetitions  = value;}, DEFAULT_REPETITIONS);
  }

  void Throbber::removed() {
    // Nothing to do.
  }

  void Throbber::updateRuntime(unsigned int milliseconds) {
    cRuntimeAnimation += milliseconds;
    while (cRuntimeAnimation >= cDefDuration) {
      cRuntimeAnimation -= cDefDuration;
    }
  }

  void Throbber::renderScreen(float scale, float aspectRatio) const {
    glDisable(GL_DEPTH_TEST);
    if (cDefShadowOffset > 0.0f) {
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      render(cDefShadowOffset, -cDefShadowOffset);
    }
    cDefColour->set();
    render(0.0f, 0.0f);
  }

  bool Throbber::renderAssetIcon() const {
    return false;
  }

  void Throbber::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Throbber::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool Throbber::isDefaultConfiguration() const {
    return true;
  }

  void Throbber::render(float xOffset, float yOffset) const {
    for (unsigned int i = 0; i < cDefSpots; ++i) {
      float mRadius = getRadius(i);
      if (mRadius > 0.0f) {
        float mAngle = -((static_cast<float>(i) * M_PI) * (2.0 / cDefSpots)) + M_PI * 2.0;
        float mX     = (std::sin(mAngle) * cDefRingRadius) + xOffset;
        float mY     = (std::cos(mAngle) * cDefRingRadius) + yOffset;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(mX, mY);
        for (unsigned int j = 0; j <= cDefSpotSides; ++j) {
          float mCircle = -((static_cast<float>(j) * M_PI) * (2.0 / cDefSpotSides)) + M_PI * 2.0;
          float mPlotX  = (std::sin(mCircle) * mRadius) + mX;
          float mPlotY  = (std::cos(mCircle) * mRadius) + mY;
          glVertex2f(mPlotX, mPlotY);
        }
        glEnd();
      }
    };
  }

  float Throbber::getRadius(unsigned int spot) const {
    unsigned int mAnimation = cRuntimeAnimation + (cDefDuration / (cDefSpots / cDefRepetitions)) * spot;
    while (mAnimation > cDefDuration) {
      mAnimation -= cDefDuration;
    }
    unsigned int mDisappear    =  cDefDuration * 0.6f;
    unsigned int mLargestStart =  mDisappear / 3U;
    unsigned int mLargestEnd   = (mDisappear / 3U) * 2U;

    if      (mAnimation < mLargestStart) {return (-((mLargestStart - mAnimation)  / static_cast<float>(mLargestStart)) + 1.0f) * cDefSpotRadius;}
    else if (mAnimation < mLargestEnd)   {return cDefSpotRadius;}
    else if (mAnimation < mDisappear)    {return ((mDisappear - mAnimation) / static_cast<float>(mDisappear - mLargestEnd)) * cDefSpotRadius;}
    else                                 {return 0.0f;}
  }
}
