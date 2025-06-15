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

#include "SequenceTrackColourEvent.h"
#include "SequenceTrackInstanceBase.h"

namespace IsoRealms::Basics {
  class SequenceTrackColour;

  class SequenceTrackColourInstance : public SequenceTrackInstanceBase<SequenceTrackColourInstance, SequenceTrackColourEvent>,
                                      public IColour {
    public:
    SequenceTrackColourInstance(SequenceTrackColour& parent, SequenceInstance& sequenceInstance);

    void positionChanged();
    void eventTriggered(SequenceTrackColourEvent& event) {}
    void eventTriggeredFrom(SequenceTrackColourEvent& event) {}
    void resetInstance() {}
    void stateChanged(IColour* colour);

    /*************************************\
      * Implements ISequenceTrackInstance *
    \*************************************/
    void registerAssets(IAssetRegistry& assets) override;
    void unregisterAssets(IAssetRemover& assets, bool relinquish) override;

    /**********************\
      * Implements IColour *
    \**********************/
    void set() const override;
    float getRed() const override;
    float getGreen() const override;
    float getBlue() const override;
    float getAlpha() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    SequenceTrackColour& cParent;

    // Runtime data.
    LiteralColour cRuntimeColour;

    // Misc.
    IStateNotifier<IColour>* cStateNotifier;

    // Internal funcctions.
    void updateColour();
    const IColour* getPreviousColour();
  };
}
