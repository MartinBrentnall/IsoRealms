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
#include "Throbber.h"

namespace IsoRealms::UI {
  const std::string Throbber::TAG_COLOUR = "Colour";

  const std::string Throbber::ATTRIBUTE_DURATION      = "duration";
  const std::string Throbber::ATTRIBUTE_REPETITIONS   = "repetitions";
  const std::string Throbber::ATTRIBUTE_RING_RADIUS   = "ringRadius";
  const std::string Throbber::ATTRIBUTE_SHADOW_OFFSET = "shadowOffset";
  const std::string Throbber::ATTRIBUTE_SPOT_RADIUS   = "spotRadius";
  const std::string Throbber::ATTRIBUTE_SPOT_SIDES    = "spotSides";
  const std::string Throbber::ATTRIBUTE_SPOTS         = "spots";

  const unsigned int Throbber::DEFAULT_DURATION      = 2000U;
  const unsigned int Throbber::DEFAULT_REPETITIONS   = 1U;
  const float        Throbber::DEFAULT_RING_RADIUS   = 0.2f;
  const float        Throbber::DEFAULT_SHADOW_OFFSET = 0.008f;
  const float        Throbber::DEFAULT_SPOT_RADIUS   = 0.06f;
  const unsigned int Throbber::DEFAULT_SPOT_SIDES    = 32U;
  const unsigned int Throbber::DEFAULT_SPOTS         = 8U;

  Throbber::Throbber(IProject* project, UI* ui) :
            cDefColour(project, 1.0f, 0.0f, 1.0f) {
    cRuntimeAnimation = 0U;

    project->updateRuntime([this](unsigned int milliseconds) {
      cRuntimeAnimation += milliseconds;
      while (cRuntimeAnimation >= cDefDuration) {
        cRuntimeAnimation -= cDefDuration;
      }
    });
  }

  Throbber::Throbber(IProject* project, UI* ui, DOMNode& node, IOptions* options, IResourceData* data) :
            Throbber(project, ui) {
    cDefDuration     = node.getIntegerAttribute(ATTRIBUTE_DURATION,    DEFAULT_DURATION);
    cDefRepetitions  = node.getIntegerAttribute(ATTRIBUTE_REPETITIONS, DEFAULT_REPETITIONS);
    cDefSpots        = node.getIntegerAttribute(ATTRIBUTE_SPOTS,       DEFAULT_SPOTS);
    cDefSpotSides    = node.getIntegerAttribute(ATTRIBUTE_SPOT_SIDES,  DEFAULT_SPOT_SIDES);
    cDefSpotRadius   = node.getFloatAttribute(ATTRIBUTE_SPOT_RADIUS,   DEFAULT_SPOT_RADIUS);
    cDefRingRadius   = node.getFloatAttribute(ATTRIBUTE_RING_RADIUS,   DEFAULT_RING_RADIUS);
    cDefShadowOffset = node.getFloatAttribute(ATTRIBUTE_SHADOW_OFFSET, DEFAULT_SHADOW_OFFSET);
    cDefColour.init(node, TAG_COLOUR);
  }

  void Throbber::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Throbbers");
  }

  void Throbber::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }

  void Throbber::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_DURATION,      cDefDuration,     DEFAULT_DURATION);
    node->addAttribute(ATTRIBUTE_REPETITIONS,   cDefRepetitions,  DEFAULT_REPETITIONS);
    node->addAttribute(ATTRIBUTE_SPOTS,         cDefSpots,        DEFAULT_SPOTS);
    node->addAttribute(ATTRIBUTE_SPOT_SIDES,    cDefSpotSides,    DEFAULT_SPOT_SIDES);
    node->addAttribute(ATTRIBUTE_SPOT_RADIUS,   cDefSpotRadius,   DEFAULT_SPOT_RADIUS);
    node->addAttribute(ATTRIBUTE_RING_RADIUS,   cDefRingRadius,   DEFAULT_RING_RADIUS);
    node->addAttribute(ATTRIBUTE_SHADOW_OFFSET, cDefShadowOffset, DEFAULT_SHADOW_OFFSET);
    cDefColour.save(node, TAG_COLOUR);
  }

  void Throbber::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Throbber::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Throbber::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void Throbber::renderScreen(float scale, float aspectRatio) const {
    if (cDefShadowOffset > 0.0f) {
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      render(cDefShadowOffset, -cDefShadowOffset);
    }
    cDefColour.set();
    render(0.0f, 0.0f);
  }

  bool Throbber::renderAssetIcon() const {
    return false;
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
