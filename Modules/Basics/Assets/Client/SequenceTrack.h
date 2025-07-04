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

#include "IsoRealms/Assets/Client/Asset.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence.h"

#include "Modules/Basics/Assets/Type/ISequenceTrack.h"

namespace IsoRealms::Basics {
  class Basics;
  class Sequence;

  class SequenceTrack : public Asset<SequenceTrack, ISequenceTrack, Basics> {
    public:
    SequenceTrack(Basics& basics, Sequence& owner);

    /********************************************\
     * Implements Asset<ISequenceTrack, Basics> *
    \********************************************/
    ISequenceTrack* createLiteralAsset(Basics& basics);
    ISequenceTrack* getAsset(Basics& basics, JSONObject object);
    ISequenceTrack* getAsset(Basics& basics, const std::string& id);
    bool isDefaultConfiguration() const;

    private:
    Sequence& cOwner;
  };
}
