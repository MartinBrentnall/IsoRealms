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
#include "ColourTrackEventPause.h"

namespace IsoRealms::Basics {
  const std::string ColourTrackEventPause::EVENT_TYPE         = "Pause";

  const std::string ColourTrackEventPause::JSON_DURATION = "duration";
  const std::string ColourTrackEventPause::JSON_TYPE     = "type";

  ColourTrackEventPause::ColourTrackEventPause(unsigned int duration) :
            cDefDuration(duration) {
  }

  void ColourTrackEventPause::save(JSONObject object) const {
    object.addString(JSON_TYPE, EVENT_TYPE);
    object.addInteger(JSON_DURATION, cDefDuration);
  }

  unsigned int ColourTrackEventPause::getDuration() const {
    return cDefDuration;
  }

  const IColour* ColourTrackEventPause::getColour() const {
    return nullptr;
  }

  LiteralColour ColourTrackEventPause::getColour(unsigned int duration, const IColour* colour) const {
    return LiteralColour(*colour);
  }

}
