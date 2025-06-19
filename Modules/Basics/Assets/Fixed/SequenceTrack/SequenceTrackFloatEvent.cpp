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
#include "SequenceTrackFloatEvent.h"

namespace IsoRealms::Basics {
  SequenceTrackFloatEvent::SequenceTrackFloatEvent(SequenceTrackFloat& parent, IResourceData& owner, IProject& project, unsigned int time, bool fade) :
            cDefTime(time),
            cDefValue(owner, 0.0f),
            cDefFade(fade) {
  }

  SequenceTrackFloatEvent::SequenceTrackFloatEvent(SequenceTrackFloat& parent, IResourceData& owner, IProject& project, JSONObject object) :
            SequenceTrackFloatEvent(parent, owner, project, object.getInteger(JSON_DURATION), object.getBoolean(JSON_FADE, true)) {
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

  std::vector<std::unique_ptr<IProperty>> SequenceTrackFloatEvent::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>("Value", "TODO", cDefValue));
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("Fade", "TODO", [this]() {return cDefFade;}, [this](bool value) {cDefFade = value;}, project));
    return mProperties;
  }

  IFloat* SequenceTrackFloatEvent::getValue() const {
    return *cDefValue;
  }

  bool SequenceTrackFloatEvent::isFade() const {
    return cDefFade;
  }

  const std::string SequenceTrackFloatEvent::JSON_DURATION = "duration";
  const std::string SequenceTrackFloatEvent::JSON_FADE     = "fade";
  const std::string SequenceTrackFloatEvent::JSON_VALUE    = "value";
}
