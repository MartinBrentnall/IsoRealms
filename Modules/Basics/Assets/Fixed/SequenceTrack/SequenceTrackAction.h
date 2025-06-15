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

#include "SequenceTrackBase.h"
#include "SequenceTrackActionEvent.h"
#include "SequenceTrackActionInstance.h"

namespace IsoRealms::Basics {
  class Sequence;

  /**
   * Action track executes actions at predetermined times in a Sequence.
   */
  class SequenceTrackAction final : public SequenceTrackBase<SequenceTrackAction, SequenceTrackActionEvent, SequenceTrackActionInstance> {
    public:
    SequenceTrackAction(IProject& project, Sequence& sequence);
    SequenceTrackAction(IProject& project, Sequence& sequence, JSONObject object);

    ISequenceTrackEvent* getEvent(unsigned int time);
    void saveAssetTrack(JSONObject object) const;

    /*****************************\
     * Implements ISequenceTrack *
    \*****************************/
    void renderIcon() const override;
    void render(float left, float bottom, float right, float top, double startTime, double endTime) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
  };
}
