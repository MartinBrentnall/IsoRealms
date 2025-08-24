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
  const std::string Throbber::JSON_COLOUR        = "colour";
  const std::string Throbber::JSON_DURATION      = "duration";
  const std::string Throbber::JSON_REPETITIONS   = "repetitions";
  const std::string Throbber::JSON_RING_RADIUS   = "ringRadius";
  const std::string Throbber::JSON_SHADOW_OFFSET = "shadowOffset";
  const std::string Throbber::JSON_SPOT_RADIUS   = "spotRadius";
  const std::string Throbber::JSON_SPOT_SIDES    = "spotSides";
  const std::string Throbber::JSON_SPOTS         = "spots";

  const unsigned int Throbber::DEFAULT_DURATION      = 2000U;
  const unsigned int Throbber::DEFAULT_REPETITIONS   = 1U;
  const float        Throbber::DEFAULT_RING_RADIUS   = 0.2f;
  const float        Throbber::DEFAULT_SHADOW_OFFSET = 0.008f;
  const float        Throbber::DEFAULT_SPOT_RADIUS   = 0.06f;
  const unsigned int Throbber::DEFAULT_SPOT_SIDES    = 32U;
  const unsigned int Throbber::DEFAULT_SPOTS         = 8U;

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

  Throbber::Throbber(UI& ui, IResourceData& data, JSONObject object) :
            Throbber(ui, data) {
    cDefDuration     = object.getInteger(JSON_DURATION,    DEFAULT_DURATION);
    cDefRepetitions  = object.getInteger(JSON_REPETITIONS, DEFAULT_REPETITIONS);
    cDefSpots        = object.getInteger(JSON_SPOTS,       DEFAULT_SPOTS);
    cDefSpotSides    = object.getInteger(JSON_SPOT_SIDES,  DEFAULT_SPOT_SIDES);
    cDefSpotRadius   = object.getFloat(JSON_SPOT_RADIUS,   DEFAULT_SPOT_RADIUS);
    cDefRingRadius   = object.getFloat(JSON_RING_RADIUS,   DEFAULT_RING_RADIUS);
    cDefShadowOffset = object.getFloat(JSON_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
    cDefColour.init(object, JSON_COLOUR);
  }

  void Throbber::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IScreen>(this, "", "Throbbers");
  }

  void Throbber::save(JSONObject object) const {
    object.addInteger(JSON_DURATION,      cDefDuration,     DEFAULT_DURATION);
    object.addInteger(JSON_REPETITIONS,   cDefRepetitions,  DEFAULT_REPETITIONS);
    object.addInteger(JSON_SPOTS,         cDefSpots,        DEFAULT_SPOTS);
    object.addInteger(JSON_SPOT_SIDES,    cDefSpotSides,    DEFAULT_SPOT_SIDES);
    object.addFloat(JSON_SPOT_RADIUS,   cDefSpotRadius,   DEFAULT_SPOT_RADIUS);
    object.addFloat(JSON_RING_RADIUS,   cDefRingRadius,   DEFAULT_RING_RADIUS);
    object.addFloat(JSON_SHADOW_OFFSET, cDefShadowOffset, DEFAULT_SHADOW_OFFSET);
    cDefColour.save(object, JSON_COLOUR);
  }

  void Throbber::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Throbber::renderIcon() const {
    return false;
  }

  void Throbber::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeUnsignedInteger(metadata.getPropertyData("Duration"),     [this]() {return cDefDuration;},     [this](unsigned int value) {cDefDuration     = value;});
    owner.createPropertyNativeUnsignedInteger(metadata.getPropertyData("Spots"),        [this]() {return cDefSpots;},        [this](unsigned int value) {cDefSpots        = value;});
    owner.createPropertyNativeUnsignedInteger(metadata.getPropertyData("SpotSides"),    [this]() {return cDefSpotSides;},    [this](unsigned int value) {cDefSpotSides    = value;});
    owner.createPropertyNativeFloat(          metadata.getPropertyData("SpotSize"),     [this]() {return cDefSpotRadius;},   [this](float        value) {cDefSpotRadius   = value;});
    owner.createPropertyNativeFloat(          metadata.getPropertyData("ShadowOffset"), [this]() {return cDefShadowOffset;}, [this](float        value) {cDefShadowOffset = value;});
    owner.createPropertyAsset<Colour>(        metadata.getPropertyData("Colour"),       cDefColour);
    owner.createPropertyNativeFloat(          metadata.getPropertyData("RingSize"),     [this]() {return cDefRingRadius;},   [this](float        value) {cDefRingRadius   = value;});
    owner.createPropertyNativeUnsignedInteger(metadata.getPropertyData("Repetitions"),  [this]() {return cDefRepetitions;},  [this](unsigned int value) {cDefRepetitions  = value;});
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

  void Throbber::getAssetProperties(PropertyMaker& owner) {
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
