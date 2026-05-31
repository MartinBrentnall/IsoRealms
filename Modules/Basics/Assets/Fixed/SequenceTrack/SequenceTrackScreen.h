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

#include "SequenceTrackBase.h"
#include "SequenceTrackScreenEvent.h"
#include "SequenceTrackScreenInstance.h"

namespace IsoRealms::Basics {
  class Sequence;

  /**
   * Track to change a screen.
   */
  class SequenceTrackScreen final : public SequenceTrackBase<SequenceTrackScreen, SequenceTrackScreenEvent, SequenceTrackScreenInstance>,
                                   public ISequenceTrackEvent {
    public:
    SequenceTrackScreen(const Metadata& metadata, Sequence& sequence);
    SequenceTrackScreen(const Metadata& metadata, Sequence& sequence, JSONObject object);

    const Screen& getStartScreen() const;
    const Metadata& getMetadata() const;
    ISequenceTrackEvent* getEvent(unsigned int time);
    void saveAssetTrack(JSONObject object) const;

    /*****************************\
     * Implements ISequenceTrack *
    \*****************************/
    void renderIcon() const override;
    void render(float left, float bottom, float right, float top, double startTime, double endTime) const override;
    void getAssetProperties(IPropertyMaker& owner) override;

    /**********************************\
      * Implements ISequenceTrackEvent *
    \**********************************/
    unsigned int getTime() const override;
    void setTime(unsigned int time) override;
    void getEventProperties(IPropertyMaker& owner) override;

    private:

    // JSON members.
    inline static const std::string JSON_START = "start";

    // External interfaces.
    const Metadata& cMetadata;

    // Definition data.
    Screen cDefStartScreen;

    void stateChanged(IScreen* screen);
  };
}

