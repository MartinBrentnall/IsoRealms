
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

#include <set>

#include <SFML/Audio.hpp>

#include "IsoRealms.h"

#include "Modules/Basics/Assets/Type/ISequenceTrack.h"

namespace IsoRealms::Basics {
  class SequenceTrackAction;

  class SequenceTrackActionEvent final : public ISequenceTrackEvent {
    public:
    SequenceTrackActionEvent(SequenceTrackAction& parent, IResourceData& owner, IProject& project, unsigned int time);
    SequenceTrackActionEvent(SequenceTrackAction& parent, IResourceData& owner, IProject& project, JSONObject object);

    void save(JSONObject object) const;
    void execute();

    /**********************************\
      * Implements ISequenceTrackEvent *
    \**********************************/
    unsigned int getTime() const override;
    void setTime(unsigned int time) override;
    void getEventProperties(PropertyMaker& owner, IProject& project) override;

    private:

      // JSON members.
    static const std::string JSON_DELAY;
    static const std::string JSON_EXECUTE;

    // Definition data.
    Action cDefAction;
    unsigned int cDefTime;
  };
}
