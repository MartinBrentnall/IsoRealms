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

#include "SequenceTrackFloatEvent.h"
#include "SequenceTrackInstanceBase.h"

namespace IsoRealms::Basics {
  class SequenceTrackFloat;

  class SequenceTrackFloatInstance : public SequenceTrackInstanceBase<SequenceTrackFloatInstance, SequenceTrackFloatEvent>,
                                     public IFloat {
    public:
    SequenceTrackFloatInstance(SequenceTrackFloat& parent, SequenceInstance& sequenceInstance);

    void positionChanged();
    void eventTriggered(SequenceTrackFloatEvent& event) {}
    void eventTriggeredFrom(SequenceTrackFloatEvent& event) {}
    void resetInstance() {}
    void stateChanged(IFloat* value);

    /*************************************\
     * Implements ISequenceTrackInstance *
    \*************************************/
    void registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) override;

    /*********************\
     * Implements IFloat *
    \*********************/
    float getValue() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    SequenceTrackFloat& cParent;

    // Runtime data.
    float cRuntimeValue;

    // Misc.
    IStateNotifier<IFloat>* cStateNotifier;

    // Internal funcctions.
    void updateValue();
    IFloat* getPreviousValue();
  };
}
