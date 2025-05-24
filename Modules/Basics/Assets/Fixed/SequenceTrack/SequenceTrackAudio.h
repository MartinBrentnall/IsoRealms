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

#include <SFML/Audio.hpp>

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
  class SequenceTrackAudio final : public ISequenceTrack {
    public:
    SequenceTrackAudio(IProject& project, Sequence& sequence, const std::string& name);
    SequenceTrackAudio(IProject& project, Sequence& sequence);
    SequenceTrackAudio(IProject& project, Sequence& sequence, JSONObject object);

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

    private:
    class Audio : public ISequenceTrackEvent {
      public:
      class End : public ISequenceTrackEvent {
        public:
        End(Audio& parent);

        /**********************************\
        * Implements ISequenceTrackEvent *
        \**********************************/
        unsigned int getTime() const override;
        void setTime(unsigned int time) override;
        std::vector<std::unique_ptr<IProperty>> getEventProperties(IProject& project) override;

        private:
        Audio& cParent;
      };

      Audio(SequenceTrackAudio& parent, IProject& project, unsigned int time);
      Audio(SequenceTrackAudio& parent, IProject& project, JSONObject object);

      End* getEndEvent();

      void play();
      void stop();
      void updateVolume();
      void playFrom(int position);

      unsigned int getDuration() const;
      int getPosition() const;

      std::string getName() const;
      void save(JSONObject object) const;

      /**********************************\
       * Implements ISequenceTrackEvent *
      \**********************************/
      unsigned int getTime() const override;
      void setTime(unsigned int time) override;
      std::vector<std::unique_ptr<IProperty>> getEventProperties(IProject& project) override;

      private:
      SequenceTrackAudio& cParent;

      End cEnd;

      unsigned int cDefTime;
      File cDefFile;

      sf::Music cMusic;
    };

    class Name : public IString {
      public:
      Name(SequenceTrackAudio& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudio& cParent;
    };

    class Count : public IInteger {
      public:
      Count(SequenceTrackAudio& parent);

      /***********************\
       * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudio& cParent;
    };

    class Current : public IInteger {
      public:
      Current(SequenceTrackAudio& parent);

      /***********************\
       * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudio& cParent;
    };

    class Length : public IString {
      public:
      Length(SequenceTrackAudio& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudio& cParent;
    };

    class Position : public IString {
      public:
      Position(SequenceTrackAudio& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudio& cParent;
    };

    // JSON members.
    static const std::string JSON_EVENTS;
    static const std::string JSON_FILE;
    static const std::string JSON_NAME;
    static const std::string JSON_TIME;
    static const std::string JSON_VOLUME;

    // Definition data.
    std::string cDefName;
    std::vector<std::unique_ptr<Audio>> cDefEvents;
    Float cDefVolume;

    // Runtime data.
    unsigned int cRuntimeEvent;
    int cRuntimePosition;
    float cRuntimeVolume;
    std::vector<int> cRuntimeEventsPlaying;

    // Exposed data.
    Name cExposedName;
    Count cExposedCount;
    Current cExposedCurrent;
    Length cExposedLength;
    Position cExposedPosition;
  };
}
