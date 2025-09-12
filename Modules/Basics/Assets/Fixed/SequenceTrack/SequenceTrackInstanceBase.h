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
  template <typename DERIVED, typename EVENT> class SequenceTrackInstanceBase : public ISequenceTrackInstance {
    public:
    SequenceTrackInstanceBase(const std::vector<std::unique_ptr<EVENT>>& events) :
              cEvents(events),
              cRuntimeEvent(0),
              cRuntimeEventPosition(0) {
    }

    unsigned int getRuntimeEvent() const {
      return cRuntimeEvent;
    }

    int getRuntimeEventPosition() {
      return cRuntimeEventPosition;
    }

    /*************************************\
     * Implements ISequenceTrackInstance *
    \*************************************/
    bool play(unsigned int milliseconds) override {
      bool mStillPlaying = false;
      if (cRuntimeEvent < cEvents.size()) {
        cRuntimeEventPosition += milliseconds;
        int mNextEventTime = cEvents[cRuntimeEvent]->getTime();
        while (cRuntimeEvent < cEvents.size() && cRuntimeEventPosition >= mNextEventTime) {
          static_cast<DERIVED*>(this)->eventTriggered(*cEvents[cRuntimeEvent].get());
          cRuntimeEvent++;
          if (cRuntimeEvent < cEvents.size()) {
            mNextEventTime = cEvents[cRuntimeEvent]->getTime();;
          }
        }
        mStillPlaying = true;
      }
      static_cast<DERIVED*>(this)->positionChanged();
      return mStillPlaying;
    }

    void reset() override {
      cRuntimeEvent         = 0;
      cRuntimeEventPosition = 0;
      static_cast<DERIVED*>(this)->resetInstance();
      static_cast<DERIVED*>(this)->positionChanged();
    }

    void stopPreview() override {
      reset();
    }

    void setPreviewPosition(long position) override {
      unsigned int mOldPosition = cRuntimeEventPosition;
      reset();
      cRuntimeEventPosition = position;
      for (const std::unique_ptr<EVENT>& mEvent : cEvents) {
        if (position >= mEvent->getTime()) {
          if (mOldPosition <= mEvent->getTime()) {
            static_cast<DERIVED*>(this)->eventTriggeredFrom(*cEvents[cRuntimeEvent].get());
          }
          cRuntimeEvent++;
        }
      }
    }

    private:
    const std::vector<std::unique_ptr<EVENT>>& cEvents;

    // Runtime data.
    unsigned int cRuntimeEvent;
    int cRuntimeEventPosition;
  };
}
