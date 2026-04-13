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
#include "SequenceTrackColourEvent.h"

#include "SequenceTrackColour.h"

namespace IsoRealms::Basics {
  SequenceTrackColourEvent::SequenceTrackColourEvent(SequenceTrackColour& parent, IResourceData& owner, unsigned int time, bool fade) :
            cParent(parent),
            cDefTime(time),
            cDefTarget(owner, 1.0f, 0.0f, 0.0f),
            cDefFade(fade) {
  }

  SequenceTrackColourEvent::SequenceTrackColourEvent(SequenceTrackColour& parent, IResourceData& owner, JSONObject object) :
            SequenceTrackColourEvent(parent, owner, object.getInteger(JSON_DURATION), object.getBoolean(JSON_FADE, true)) {
    cDefTarget.init(object, JSON_TARGET);
  }

  void SequenceTrackColourEvent::save(JSONObject object) const {
    object.addInteger(JSON_DURATION, cDefTime);
    object.addBoolean(JSON_FADE, cDefFade, true);
    cDefTarget.save(object, JSON_TARGET);
  }

  unsigned int SequenceTrackColourEvent::getTime() const {
    return cDefTime;
  }

  void SequenceTrackColourEvent::setTime(unsigned int time) {
    cDefTime = time;
  }

  void SequenceTrackColourEvent::getEventProperties(IPropertyMaker& owner) {
    const Metadata& mMetadata = cParent.getMetadata();
    owner.createPropertyTreeSelector( mMetadata.getPropertyData("Colour"), cDefTarget);
    owner.createPropertyNativeBoolean(mMetadata.getPropertyData("Fade"),   [this]() {return cDefFade;}, [this](bool fade) {cDefFade = fade;});
  }

  const IColour* SequenceTrackColourEvent::getColour() const {
    return *cDefTarget;
  }

  bool SequenceTrackColourEvent::isFade() const {
    return cDefFade;
  }

  const std::string SequenceTrackColourEvent::JSON_DURATION = "duration";
  const std::string SequenceTrackColourEvent::JSON_FADE     = "fade";
  const std::string SequenceTrackColourEvent::JSON_TARGET   = "target";
}
