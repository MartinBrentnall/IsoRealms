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
#pragma once

#include "IsoRealms/Types.h"

#include "IColourTrackEvent.h"

namespace IsoRealms::Basics {

  /**
   * Colour track event that fades for a duration from the previous colour to
   * a specified colour.
   */
  class ColourTrackEventFade : public IColourTrackEvent {
    public:
    static const std::string EVENT_TYPE;

    // Constructor.
    ColourTrackEventFade(IProject* project, unsigned int duration, JSONObject object);

    /********************************\
     * Implements IColourTrackEvent *
    \********************************/
    void save(JSONObject object) const override;
    unsigned int getDuration() const override;
    const IColour* getColour() const override;
    LiteralColour getColour(unsigned int duration, const IColour* previousColour) const override;
    
    private:

    // JSON members.
    static const std::string JSON_DURATION;
    static const std::string JSON_TARGET;
    static const std::string JSON_TYPE;

    // Definition data.
    unsigned int cDefDuration;
    Colour cDefTarget;
  };
}
