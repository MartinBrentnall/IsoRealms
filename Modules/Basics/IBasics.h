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

#include <string>
#include <vector>

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/Persistence/JSONDocument.h"

namespace IsoRealms::Basics {
  class ISequenceTrack;
  class Sequence;

  class IBasics {
    public:
    virtual std::vector<std::string> getAllSequenceTracks() = 0;

    virtual std::string getID(const ISequenceTrack* asset) const = 0;

    virtual bool renderSequenceTrackIcon(const std::string& id) const = 0;

    virtual bool isSequenceTrackConfigurable(const std::string& id) const = 0;

    virtual ISequenceTrack* createLiteralSequenceTrack(IAssetUser<ISequenceTrack>* user, Sequence& owner) = 0;

    virtual ISequenceTrack* getSequenceTrack(IAssetUser<ISequenceTrack>* user, JSONObject object, Sequence& owner) = 0;

    virtual ISequenceTrack* getSequenceTrack(IAssetUser<ISequenceTrack>* user, const std::string& id, Sequence& owner) = 0;

    virtual void release(IAssetUser<ISequenceTrack>* user, ISequenceTrack* asset) = 0;

    virtual void save(JSONObject object, ISequenceTrack* asset) const = 0;

    virtual IProject& getProject() const = 0;

    virtual ~IBasics() {}
  };
}

