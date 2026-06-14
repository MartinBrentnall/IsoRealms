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

#include "IsoRealms.h"

#include "Modules/Basics/Resources/Fixed/SequenceTrack/SequenceTrackAction.h"
#include "Modules/Basics/Resources/Fixed/SequenceTrack/SequenceTrackAudio.h"
#include "Modules/Basics/Resources/Fixed/SequenceTrack/SequenceTrackColour.h"
#include "Modules/Basics/Resources/Fixed/SequenceTrack/SequenceTrackFloat.h"
#include "Modules/Basics/Resources/Fixed/SequenceTrack/SequenceTrackScreen.h"
#include "Modules/Basics/Resources/Type/ISequenceTrack.h"
#include "Modules/Basics/Sequence/Sequence.h"

namespace IsoRealms::Basics {
  class Sequence;

  class SequenceTrackRegistry : public ResourceClientManager<SequenceTrackRegistry, Sequence, ISequenceTrack> {
    public:
    SequenceTrackRegistry(IComponentTypeRegistry& registry);

    private:
    ResourceInstanced<Sequence, ISequenceTrack, SequenceTrackAction> cAction;
    ResourceInstanced<Sequence, ISequenceTrack, SequenceTrackAudio>  cAudio;
    ResourceInstanced<Sequence, ISequenceTrack, SequenceTrackColour> cColour;
    ResourceInstanced<Sequence, ISequenceTrack, SequenceTrackFloat>  cFloat;
    ResourceInstanced<Sequence, ISequenceTrack, SequenceTrackScreen> cScreen;
  };
}
