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

namespace IsoRealms::Basics {

  /**
    * Interface for a sequence track.
    */
  class ISequenceTrackInstance {
    public:

    /**
      * Register resources belonging to this track.
      *
      * @param publisher Interface for resource registration.
      */
    virtual void publish(ResourcePublisher& publisher, const std::string& parentID) = 0;

    /**
      * Forward this track and perform processing according to events within
      * the track.
      *
      * @param milliseconds The amount to forward by.
      * @return true if there are still events to process within the track.
      */
    virtual bool play(unsigned int milliseconds) = 0;

    /**
      * Reset this track to its initial state.
      */
    virtual void reset() = 0;

    virtual void stopPreview() = 0;
    virtual void setPreviewPosition(long position) = 0;

    virtual ~ISequenceTrackInstance() {}
  };
}
