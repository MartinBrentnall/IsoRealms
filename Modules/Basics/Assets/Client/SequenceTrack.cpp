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
#include "SequenceTrack.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  SequenceTrack::SequenceTrack(Basics& basics, Sequence& sequence) :
            Asset<SequenceTrack, ISequenceTrack, Basics>(basics, basics.getSequenceTrack(this, "Colour", sequence)),
            cOwner(sequence) {
  }

  ISequenceTrack* SequenceTrack::createLiteralAsset(Basics& basics) {
    return basics.getSequenceTrack(this, "Colour", cOwner);
  }
  
  ISequenceTrack* SequenceTrack::getAsset(Basics& basics, JSONObject object) {
    return basics.getSequenceTrack(this, object, cOwner);
  }
  
  ISequenceTrack* SequenceTrack::getAsset(Basics& basics, const std::string& id) {
    return basics.getSequenceTrack(this, id, cOwner);
  }
  
  bool SequenceTrack::isDefaultConfiguration() const {
    return true;
  }
}
