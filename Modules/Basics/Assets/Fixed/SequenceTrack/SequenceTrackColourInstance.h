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
    void registerAssets(ComponentAssetRegistry& assets, const std::string& parentID) override;

    /**********************\
      * Implements IColour *
    \**********************/
    void set() const override;
    float getRed() const override;
    float getGreen() const override;
    float getBlue() const override;
    float getAlpha() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    SequenceTrackColour& cParent;

    // Runtime data.
    LiteralColour cRuntimeColour;

    // Misc.
    IStateNotifier* cStateNotifier = nullptr;

    // Internal funcctions.
    void updateColour();
    const IColour* getPreviousColour();
  };
}
