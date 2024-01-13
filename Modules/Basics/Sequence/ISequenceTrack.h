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

#include "IsoRealms/ResourceDefinition.h"

namespace IsoRealms::Basics {

  /**
    * Interface for a sequence track.
    */
  class ISequenceTrack {
    public:

    /**
      * Register assets belonging to this track.
      *
      * @param assets Interface for asset registration.
      */
    virtual void registerAssets(IAssetRegistry* assets) = 0;

    /**
      * Unregister assets belonging to this track.
      *
      * @param assets Interface for asset unregistration.
      */
    virtual void unregisterAssets(IAssetRemover* assets) = 0;

    /**
      * Save this track.
      *
      * @param node The node to write this track to.
      */
    virtual void save(DOMNodeWriter* node) const = 0;

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

    /**
      * Get the number of milliseconds it will take in total to process all
      * events within this track.
      *
      * @return Duration of the track in milliseconds.
      */
    virtual unsigned int getDuration() const = 0;

    /**
      * Render a visual representation of the track for editing, fitting
      * within the specified boundary.
      *
      * @param left Left boundary of the visual representation.
      * @param bottom Bottom boundary of the visual representation.
      * @param right Right boundary of the visual representation.
      * @param top Top boundary of the visual representation.
      */
    virtual void render(float left, float bottom, float right, float top) const = 0;

    virtual ~ISequenceTrack() {}
  };
}
