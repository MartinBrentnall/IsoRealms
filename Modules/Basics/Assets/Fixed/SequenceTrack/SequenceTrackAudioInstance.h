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

#include "Modules/Basics/Sequence/Sequence.h"

#include "SequenceTrackAudioEvent.h"
#include "SequenceTrackInstanceBase.h"

namespace IsoRealms::Basics {
  class SequenceTrackAudio;

  class SequenceTrackAudioInstance : public SequenceTrackInstanceBase<SequenceTrackAudioInstance, SequenceTrackAudioEvent> {
    public:
    SequenceTrackAudioInstance(SequenceTrackAudio& parent, SequenceInstance& sequenceInstance);

    void positionChanged();
    void eventTriggered(SequenceTrackAudioEvent& event);
    void eventTriggeredFrom(SequenceTrackAudioEvent& event);
    void resetInstance();

    /***********************\
     * Scripting interface *
    \***********************/
    void nextTrack();
    void previousTrack();
    void jumpToTrack(int track);

    /*************************************\
     * Implements ISequenceTrackInstance *
    \*************************************/
    void registerAssets(IAssetRegistry& assets) override;
    void unregisterAssets(IAssetRemover& assets, bool relinquish) override;

    private:
    class Name : public IString {
      public:
      Name(SequenceTrackAudioInstance& parent);

      /**********************\
      * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudioInstance& cParent;
    };

    class Count : public IInteger {
      public:
      Count(SequenceTrackAudioInstance& parent);

      /***********************\
      * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudioInstance& cParent;
    };

    class Current : public IInteger {
      public:
      Current(SequenceTrackAudioInstance& parent);

      /***********************\
      * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudioInstance& cParent;
    };

    class Length : public IInteger {
      public:
      Length(SequenceTrackAudioInstance& parent);

      /**********************\
      * Implements IInteger *
      \**********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudioInstance& cParent;
    };

    class Position : public IInteger {
      public:
      Position(SequenceTrackAudioInstance& parent);

      /**********************\
      * Implements IInteger *
      \**********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceTrackAudioInstance& cParent;
    };

    // External interfaces.
    SequenceTrackAudio& cParent;
    SequenceInstance& cSequenceInstance;

    // Runtime data.
    float cRuntimeVolume;
    std::vector<int> cRuntimeEventsPlaying;

    // Exposed data.
    Name cExposedName;
    Count cExposedCount;
    Current cExposedCurrent;
    Length cExposedLength;
    Position cExposedPosition;

    // Scripting interface.
    LuaBinding<SequenceTrackAudioInstance> cLuaBinding;
  };
}
