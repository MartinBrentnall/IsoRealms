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
#include "SequenceTrack.h"

#include "IsoRealms/Editing/Property/IProperty.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  SequenceTrack::SequenceTrack(IBasics& basics, Sequence& sequence) :
            Asset<ISequenceTrack, IBasics>(basics, basics.getSequenceTrack(this, "Colour", sequence)),
            cOwner(sequence) {
  }

  ISequenceTrack* SequenceTrack::createLiteralAsset(IBasics& basics) {
    return basics.createLiteralSequenceTrack(this, cOwner);
  }
  
  ISequenceTrack* SequenceTrack::getAsset(IBasics& basics, JSONObject object) {
    return basics.getSequenceTrack(this, object, cOwner);
  }
  
  ISequenceTrack* SequenceTrack::getAsset(IBasics& basics, const std::string& id) {
    return basics.getSequenceTrack(this, id, cOwner);
  }
  
  std::vector<std::string> SequenceTrack::getAvailableProviders() const {
    return cManager.getAllSequenceTracks();
  }  

  bool SequenceTrack::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderSequenceTrackIcon(id);
  }

  bool SequenceTrack::hasConfiguration() const {
    return cManager.isSequenceTrackConfigurable(getID());
  }

  bool SequenceTrack::isDefaultConfiguration() const {
    return true;
  }
}
