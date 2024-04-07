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
#include "ColourTrackEventFade.h"

namespace IsoRealms::Basics {
  const std::string ColourTrackEventFade::EVENT_TYPE         = "Fade";

  const std::string ColourTrackEventFade::TAG_TARGET         = "Target";

  const std::string ColourTrackEventFade::ATTRIBUTE_DURATION = "duration";

  ColourTrackEventFade::ColourTrackEventFade(IProject* project, unsigned int duration, DOMNode& node) :
            cDefDuration(duration),
            cDefTarget(project, 1.0f, 0.0f, 0.0f) {
    cDefTarget.init(node, TAG_TARGET);
  }
  
  void ColourTrackEventFade::save(DOMNodeWriter* node) const {
    DOMNodeWriter mFadeNode = node->addBranch(EVENT_TYPE);
    mFadeNode.addAttribute(ATTRIBUTE_DURATION, cDefDuration);
    cDefTarget.save(&mFadeNode, TAG_TARGET);
  }

  unsigned int ColourTrackEventFade::getDuration() const {
    return cDefDuration;
  }

  const IColour* ColourTrackEventFade::getColour() const {
    return &cDefTarget;
  }

  LiteralColour ColourTrackEventFade::getColour(unsigned int position, const IColour* colour) const {
    float mRelativePosition = position / static_cast<float>(cDefDuration);
    return LiteralColour(*colour, cDefTarget, mRelativePosition);
  }
}
