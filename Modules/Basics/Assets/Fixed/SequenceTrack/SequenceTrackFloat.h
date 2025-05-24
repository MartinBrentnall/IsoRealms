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

#include <set>

#include "IsoRealms/Literals.h"
#include "IsoRealms/LocalAssetRegistry.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "Modules/Basics/Assets/Type/ISequenceTrack.h"

namespace IsoRealms::Basics {
  class Sequence;

  /**
   * Track to change a numeric value.
   */
  class SequenceTrackFloat final : public ISequenceTrack,
                                   public ISequenceTrackEvent {
    public:
    SequenceTrackFloat(IProject& project, Sequence& sequence);
    SequenceTrackFloat(IProject& project, Sequence& sequence, JSONObject object);

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

    /**********************************\
      * Implements ISequenceTrackEvent *
    \**********************************/
    unsigned int getTime() const override;
    void setTime(unsigned int time) override;
    std::vector<std::unique_ptr<IProperty>> getEventProperties(IProject& project) override;

    private:

    class Output : public IFloat {
      public:
      Output(SequenceTrackFloat& parent);

      /*********************\
       * Implements IFloat *
      \*********************/
      float getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackFloat& cParent;
    };

    class Event : public ISequenceTrackEvent {
      public:
      Event(IProject& project, unsigned int time, bool fade);
      Event(IProject& project, JSONObject object);

      void save(JSONObject object) const;
      float getValue() const;
      bool isFade() const;

      /**********************************\
       * Implements ISequenceTrackEvent *
      \**********************************/
      unsigned int getTime() const override;
      void setTime(unsigned int time) override;
      std::vector<std::unique_ptr<IProperty>> getEventProperties(IProject& project) override;

      private:
      unsigned int cDefTime;
      Float cDefValue;
      bool cDefFade;
    };

    // JSON members.
    static const std::string JSON_DURATION;
    static const std::string JSON_EVENTS;
    static const std::string JSON_FADE;
    static const std::string JSON_OUTPUT;
    static const std::string JSON_START;
    static const std::string JSON_VALUE;

    Output cOutput;

    // Definition data.
    std::string cDefName;
    Float cDefStartValue;
    std::vector<std::unique_ptr<Event>> cDefEvents;

    // Runtime data.
    float cRuntimeValue;
    unsigned int cRuntimeEvent;
    int cRuntimeEventPosition;

    // Misc.
    IStateNotifier<IFloat>* cStateNotifier;

    /**********************\
     * Internal Functions *
    \**********************/
    void updateValue();
    float getPreviousValue();
    void stateChanged(float value);
  };
}
