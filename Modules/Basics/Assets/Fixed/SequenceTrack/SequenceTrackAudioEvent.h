
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

#include <SFML/Audio.hpp>

#include "IsoRealms.h"

#include "Modules/Basics/Assets/Type/ISequenceTrack.h"

namespace IsoRealms::Basics {
  class SequenceTrackAudio;

  class SequenceTrackAudioEvent : public ISequenceTrackEvent {
    public:
    class End : public ISequenceTrackEvent {
      public:
      End(SequenceTrackAudioEvent& parent);

      /**********************************\
      * Implements ISequenceTrackEvent *
      \**********************************/
      unsigned int getTime() const override;
      void setTime(unsigned int time) override;
      std::vector<std::unique_ptr<IProperty>> getEventProperties(IPropertyOwner& owner, IProject& project) override;

      private:
      SequenceTrackAudioEvent& cParent;
    };

    SequenceTrackAudioEvent(SequenceTrackAudio& parent, IResourceData& owner, IProject& project, unsigned int time);
    SequenceTrackAudioEvent(SequenceTrackAudio& parent, IResourceData& owner, IProject& project, JSONObject object);

    End* getEndEvent();

    void play();
    void stop();
    void updateVolume(float volume);
    void playFrom(int position, float volume);

    unsigned int getDuration() const;
    int getPosition() const;

    std::string getName() const;
    void save(JSONObject object) const;

    /**********************************\
      * Implements ISequenceTrackEvent *
    \**********************************/
    unsigned int getTime() const override;
    void setTime(unsigned int time) override;
    std::vector<std::unique_ptr<IProperty>> getEventProperties(IPropertyOwner& owner, IProject& project) override;

    private:
    static const std::string JSON_FILE;
    static const std::string JSON_TIME;

    SequenceTrackAudio& cParent;

    End cEnd;

    unsigned int cDefTime;
    File cDefFile;

    sf::Music cMusic;
  };
}
