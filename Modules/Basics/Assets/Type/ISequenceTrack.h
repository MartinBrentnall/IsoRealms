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

#include "IsoRealms.h"

#include "ISequenceTrackEvent.h"
#include "ISequenceTrackInstance.h"

namespace IsoRealms::Basics {
  class SequenceInstance;

  /**
    * Interface for a sequence track.
    */
  class ISequenceTrack : public IAsset {
    public:

    /**
      * Get the number of milliseconds it will take in total to process all
      * events within this track.
      *
      * @return Duration of the track in milliseconds.
      */
    virtual unsigned int getDuration() const = 0;
    virtual void setName(const std::string& name) = 0;
    virtual std::string getName() const = 0;

    virtual ISequenceTrackEvent* createEvent(IComponentData& owner, unsigned int time) = 0;
    virtual void deleteEvent(ISequenceTrackEvent* event) = 0;
    virtual void setEventTime(ISequenceTrackEvent* event, unsigned int time) = 0;
    virtual std::vector<ISequenceTrackEvent*> getEvents() = 0;

    /**
     * Render icon representing this track.
     */
    virtual void renderIcon() const = 0;

    virtual ISequenceTrackInstance* createTrackInstance(SequenceInstance& sequenceInstance) = 0;

    /**
      * Render a visual representation of the track for editing, fitting
      * within the specified boundary.
      *
      * @param left Left boundary of the visual representation.
      * @param bottom Bottom boundary of the visual representation.
      * @param right Right boundary of the visual representation.
      * @param top Top boundary of the visual representation.
      */
    virtual void render(float left, float bottom, float right, float top, double startTime, double endTime) const = 0;

    virtual ~ISequenceTrack() {}
  };
}
