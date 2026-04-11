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

#include <iostream>
#include <vector>

#include "IsoRealms.h"

#include "Modules/Basics/Assets/Client/SequenceTrack.h"

namespace IsoRealms::Basics {
  class Sequence;

  class SequenceInstance {
    public:
    SequenceInstance(Sequence& parent, int startTime, float speed);
    SequenceInstance(Sequence& parent);
    SequenceInstance(Sequence& parent, JSONObject object);

    void addTrackInstance(ISequenceTrackInstance* trackInstance);

    Sequence& getSequence() const;

    void registerAssets(ResourceAssetRegistry& assets, const std::string& parentID);
    void stopPreview();
    void setPreviewPosition(long position);
    void update(unsigned int milliseconds);
    void updatePreview(unsigned int milliseconds);
    void save(JSONObject object) const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

    /***********************\
     * Scripting Interface *
    \***********************/
    void play();
    void pause();
    void reset();
    int getTime() const;
    void setTime(int time);

    void deleteTrackInstance(unsigned int track);
    
    private:
    static const std::string JSON_SPEED;
    static const std::string JSON_START_TIME;

    class Position : public IInteger {
      public:
      Position(SequenceInstance& parent);

      /***********************\
        * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceInstance& cParent;
    };

    class Remaining : public IInteger {
      public:
      Remaining(SequenceInstance& parent);

      /***********************\
        * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      SequenceInstance& cParent;
    };

    // External interfaces.
    Sequence& cParent;

    // Definition data.
    int cDefStartTime;
    float cDefSpeed;

    // Runtime data.
    std::vector<ISequenceTrackInstance*> cTrackInstances;
    int cRuntimePosition;
    float cRuntimePositionFraction;
    bool cRuntimePlaying; /// Current state of this sequence.

    // Exposed data.
    Position cExposedPosition;
    Remaining cExposedRemaining;

    // Scripting interface.
    LuaBinding<SequenceInstance> cLuaBinding;
  };
}

