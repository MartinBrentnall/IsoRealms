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
  class Sequence;

  template <typename DERIVED, typename EVENT, typename INSTANCE> class SequenceTrackBase : public ISequenceTrack {
    public:
    SequenceTrackBase(Sequence& sequence) :
              cSequence(sequence),
              cDefName("Unnamed Track") {
    }

    SequenceTrackBase(IResourceData& owner, Sequence& sequence, JSONObject object) :
              cSequence(sequence),
              cDefName(object.getString(JSON_NAME)) {
      for (JSONValue mEventValue : object.getArray(JSON_EVENTS)) {
        cDefEvents.push_back(std::make_unique<EVENT>(*static_cast<DERIVED*>(this), owner, mEventValue.getObject()));
      }
    }

    /*****************************\
     * Implements ISequenceTrack *
    \*****************************/
    const std::vector<std::unique_ptr<EVENT>>& getRealEvents() const {
      return cDefEvents;
    }

    unsigned int getDuration() const override {
      return cDefEvents.empty() ? 0 : cDefEvents[cDefEvents.size() - 1]->getTime();
    }

    Sequence& getSequence() const {
      return cSequence;
    }

    void setName(const std::string& name) override {
      cDefName = name;
    }

    std::string getName() const override {
      return cDefName;
    }

    ISequenceTrackEvent* createEvent(IResourceData& owner, unsigned int time) override {
      ISequenceTrackEvent* mTrackSpecificEvent = static_cast<DERIVED*>(this)->getEvent(time);
      if (mTrackSpecificEvent != nullptr) {
        return mTrackSpecificEvent;
      }
      for (unsigned int i = 0; i < cDefEvents.size(); i++) {
        if (cDefEvents[i]->getTime() == time) {
          return cDefEvents[i].get();
        } else if (cDefEvents[i]->getTime() > time) {
          return cDefEvents.insert(cDefEvents.begin() + i, std::make_unique<EVENT>(*static_cast<DERIVED*>(this), owner, time))->get();
        }
      }
      return cDefEvents.emplace_back(std::make_unique<EVENT>(*static_cast<DERIVED*>(this), owner, time)).get();
    }

    void deleteEvent(ISequenceTrackEvent* event) override {
      Utils::removeElementUnique(cDefEvents, event);
    }

    void setEventTime(ISequenceTrackEvent* event, unsigned int time) override {
      int mEventIndex = Utils::getIndex(cDefEvents, event);
      int mNewIndex = cDefEvents.size();
      for (unsigned int i = 0; i < cDefEvents.size(); i++) {
        if (cDefEvents[i]->getTime() >= time) {
          mNewIndex = i;
          break;
        }
      }

      std::unique_ptr<EVENT> mEventToMove = nullptr;
      if (mNewIndex < mEventIndex) {
        mEventToMove = std::move(cDefEvents[mEventIndex]);
        cDefEvents.erase(cDefEvents.begin() + mEventIndex);
      } else if (mNewIndex > mEventIndex + 1) {
        mEventToMove = std::move(cDefEvents[mEventIndex]);
        cDefEvents.erase(cDefEvents.begin() + mEventIndex);
        mNewIndex--;
      }

      if (mEventToMove != nullptr) {
        cDefEvents.insert(cDefEvents.begin() + mNewIndex, std::move(mEventToMove));
      }
      event->setTime(time);
    }

    std::vector<ISequenceTrackEvent*> getEvents() override {
      std::vector<ISequenceTrackEvent*> mEvents;
      ISequenceTrackEvent* mTrackStartEvent = static_cast<DERIVED*>(this)->getEvent(0);
      if (mTrackStartEvent != nullptr) {
        mEvents.emplace_back(mTrackStartEvent);
      }
      for (const std::unique_ptr<EVENT>& mEvent : cDefEvents) {
        mEvents.emplace_back(mEvent.get());
      }
      return mEvents;
    }

    std::vector<std::unique_ptr<INSTANCE>>& getInstances() {
      return cInstances ;
    }

    ISequenceTrackInstance* createTrackInstance(SequenceInstance& sequenceInstance) override {
      return cInstances.emplace_back(std::make_unique<INSTANCE>(static_cast<DERIVED&>(*this), sequenceInstance)).get();
    }

    /****************************************\
     * Implements IAsset via ISequenceTrack *
    \****************************************/
    bool renderAssetIcon() const override {
      return false;
    }

    void saveAsset(JSONObject object) const override {
      object.addString(JSON_NAME, cDefName);
      static_cast<const DERIVED*>(this)->saveAssetTrack(object);
      JSONArray mEventsArray = object.addArray(JSON_EVENTS);
      for (const std::unique_ptr<EVENT>& mEvent : cDefEvents) {
        JSONObject mEventObject = mEventsArray.addObject();
        mEvent->save(mEventObject);
      }
    }

    bool isDefaultConfiguration() const override {
      return true;
    }

    private:


    // JSON members.
    inline static const std::string JSON_EVENTS = "events";
    inline static const std::string JSON_NAME   = "name";

    // External interfaces.
    Sequence& cSequence;

    // Definition data.
    std::string cDefName;
    std::vector<std::unique_ptr<EVENT>> cDefEvents;

    // Runtime data.
    std::vector<std::unique_ptr<INSTANCE>> cInstances;
  };
}
