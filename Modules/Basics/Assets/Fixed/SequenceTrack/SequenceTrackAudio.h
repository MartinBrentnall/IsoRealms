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
#include "IsoRealms/Lua.h"
#include "IsoRealms/LocalAssetRegistry.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "SequenceTrackBase.h"
#include "SequenceTrackAudioEvent.h"
#include "SequenceTrackAudioInstance.h"

namespace IsoRealms::Basics {
  class Sequence;

  /**
   * Track to change a numeric value.
   */
  class SequenceTrackAudio final : public SequenceTrackBase<SequenceTrackAudio, SequenceTrackAudioEvent, SequenceTrackAudioInstance> {
    public:
    SequenceTrackAudio(IProject& project, Sequence& sequence);
    SequenceTrackAudio(IProject& project, Sequence& sequence, JSONObject object);

    float getVolume() const;
    ISequenceTrackEvent* getEvent(unsigned int time);
    void saveAssetTrack(JSONObject object) const;

    /*****************************\
     * Implements ISequenceTrack *
    \*****************************/
    void renderIcon() const override;
    void render(float left, float bottom, float right, float top, double startTime, double endTime) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;

    private:

    // JSON members.
    static const std::string JSON_EVENTS;
    static const std::string JSON_FILE;
    static const std::string JSON_NAME;
    static const std::string JSON_TIME;
    static const std::string JSON_VOLUME;

    // Definition data.
    Float cDefVolume;
  };
}
