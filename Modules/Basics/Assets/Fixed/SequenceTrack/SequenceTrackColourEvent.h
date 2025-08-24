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

#include "IsoRealms.h"

#include "Modules/Basics/Assets/Type/ISequenceTrack.h"

namespace IsoRealms::Basics {
  class SequenceTrackColour;

  class SequenceTrackColourEvent : public ISequenceTrackEvent {
    public:
    SequenceTrackColourEvent(SequenceTrackColour& parent, IResourceData& owner, unsigned int time, bool fade = true);
    SequenceTrackColourEvent(SequenceTrackColour& parent, IResourceData& owner, JSONObject object);

    void save(JSONObject object) const;
    const IColour* getColour() const;
    bool isFade() const;

    /**********************************\
      * Implements ISequenceTrackEvent *
    \**********************************/
    unsigned int getTime() const override;
    void setTime(unsigned int time) override;
    void getEventProperties(PropertyMaker& owner, const Metadata& metadata) override;

    private:

    static const std::string JSON_DURATION;
    static const std::string JSON_FADE;
    static const std::string JSON_TARGET;

    // Definition data.
    unsigned int cDefTime;
    Colour cDefTarget;
    bool cDefFade;
  };
}
