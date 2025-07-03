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
#include "SequenceTrackActionEvent.h"

#include "SequenceTrackAction.h"

namespace IsoRealms::Basics {
  SequenceTrackActionEvent::SequenceTrackActionEvent(SequenceTrackAction& parent, IResourceData& owner, IProject& project, unsigned int time) :
            cDefAction(owner.getDummyActionClient()),
            cDefTime(time) {
  }

  SequenceTrackActionEvent::SequenceTrackActionEvent(SequenceTrackAction& parent, IResourceData& owner, IProject& project, JSONObject object) :
            SequenceTrackActionEvent(parent, owner, project, object.getInteger(JSON_DELAY)) {
    cDefAction.init(object, JSON_EXECUTE);
  }

  unsigned int SequenceTrackActionEvent::getTime() const {
    return cDefTime;
  }

  void SequenceTrackActionEvent::setTime(unsigned int time) {
    cDefTime = time;
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackActionEvent::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Action", "Action to be performed upon reaching this event", cDefAction));
    return mProperties;
  }

  void SequenceTrackActionEvent::save(JSONObject object) const {
    object.addInteger(JSON_DELAY, cDefTime);
    cDefAction.save(object, JSON_EXECUTE);
  }

  void SequenceTrackActionEvent::execute() {
    cDefAction.execute();
  }

  const std::string SequenceTrackActionEvent::JSON_DELAY   = "delay";
  const std::string SequenceTrackActionEvent::JSON_EXECUTE = "execute";
}
