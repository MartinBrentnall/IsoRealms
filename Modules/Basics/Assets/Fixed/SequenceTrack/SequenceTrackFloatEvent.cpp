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
#include "SequenceTrackFloatEvent.h"

#include "SequenceTrackFloat.h"

namespace IsoRealms::Basics {
  SequenceTrackFloatEvent::SequenceTrackFloatEvent(SequenceTrackFloat& parent, IResourceData& owner, unsigned int time, bool fade) :
            cParent(parent),
            cDefTime(time),
            cDefValue(owner, 0.0f),
            cDefFade(fade) {
  }

  SequenceTrackFloatEvent::SequenceTrackFloatEvent(SequenceTrackFloat& parent, IResourceData& owner, JSONObject object) :
            SequenceTrackFloatEvent(parent, owner, object.getInteger(JSON_DURATION), object.getBoolean(JSON_FADE, true)) {
    cDefValue.init(object, JSON_VALUE);
  }

  void SequenceTrackFloatEvent::save(JSONObject object) const {
    object.addInteger(JSON_DURATION, cDefTime);
    object.addBoolean(JSON_FADE, cDefFade, true);
    cDefValue.save(object, JSON_VALUE);
  }

  unsigned int SequenceTrackFloatEvent::getTime() const {
    return cDefTime;
  }

  void SequenceTrackFloatEvent::setTime(unsigned int time) {
    cDefTime = time;
  }

  void SequenceTrackFloatEvent::getEventProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_VALUE, cDefValue);
    owner.createPropertyNativeBoolean(JSON_FADE, [this]() {return cDefFade;}, [this](bool value) {cDefFade = value;});
  }

  IFloat* SequenceTrackFloatEvent::getValue() const {
    return *cDefValue;
  }

  bool SequenceTrackFloatEvent::isFade() const {
    return cDefFade;
  }
}
