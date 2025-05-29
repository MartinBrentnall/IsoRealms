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

#include "IsoRealms/Editing.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "Modules/Basics/Assets/Type/ISequenceTrack.h"

namespace IsoRealms::Basics {
  class Sequence;

  /**
   * Action track executes actions at predetermined times in a Sequence.
   */
  class SequenceTrackAction final : public ISequenceTrack {
    public:
    SequenceTrackAction(IProject& project, Sequence& sequence);
    SequenceTrackAction(IProject& project, Sequence& sequence, JSONObject object);

    /*****************************\
     * Implements ISequenceTrack *
    \*****************************/
    void registerAssets(IAssetRegistry& assets) override;
    void unregisterAssets(IAssetRemover& assets, bool relinquish) override;
    bool play(unsigned int milliseconds) override;
    void reset() override;
    unsigned int getDuration() const override;
    void setName(const std::string& name) override;
    std::string getName() const override;
    ISequenceTrackEvent* createEvent(IProject& project, unsigned int time) override;
    void deleteEvent(ISequenceTrackEvent* event) override;
    void setEventTime(ISequenceTrackEvent* event, unsigned int time) override;
    std::vector<ISequenceTrackEvent*> getEvents() override;
    void stopPreview() override;
    void setPreviewPosition(long position) override;
    void renderIcon() const override;
    void render(float left, float bottom, float right, float top, double startTime, double endTime) const override;

    /****************************************\
     * Implements IAsset via ISequenceTrack *
    \****************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_DELAY;
    static const std::string JSON_EVENTS;
    static const std::string JSON_EXECUTE;

    // Definition data.
    class Event final : public ISequenceTrackEvent {
      public:
      Event(JSONObject object, IProject& project, unsigned int prevTimes);

      void save(JSONObject object) const;
      void execute();

      /**********************************\
       * Implements ISequenceTrackEvent *
      \**********************************/
      unsigned int getTime() const override;
      void setTime(unsigned int time) override;
      std::vector<std::unique_ptr<IProperty>> getEventProperties(IProject& project) override;

      private:
      Action cDefAction;
      unsigned int cDefTime;
    };
    std::vector<std::unique_ptr<Event>> cDefEvents;

    // Runtime data.
    unsigned int cRuntimeAction;
    unsigned int cRuntimeActionRemaining;
  };
}
