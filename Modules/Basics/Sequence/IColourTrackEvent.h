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

#include "IsoRealms/Literals.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {

  /**
   * Interface for an event within a colour track.
   */
  class IColourTrackEvent {
    public:

    /**
     * Save the event to the specified node.
     *
     * @param node Node to save the event to.
     */
    virtual void save(DOMNodeWriter* node) const = 0;

    /**
     * Get the duration of this event.
     *
     * @return The duration of this event.
     */
    virtual unsigned int getDuration() const = 0;

    /**
     * Get the target colour of this event.  May be nullptr if the event has
     * no target colour.
     */
    virtual const IColour* getColour() const = 0;

    /**
     * Get a colour of this event derived from the colour of the previous event
     * and the position within this event.  This is useful when fading from one
     * colour to another.
     */
    virtual LiteralColour getColour(unsigned int position, const IColour* previousColour) const = 0;

    virtual ~IColourTrackEvent() {}
  };
}
