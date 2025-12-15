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

#include "SequenceTrackScreenEvent.h"
#include "SequenceTrackInstanceBase.h"

namespace IsoRealms::Basics {
  class SequenceTrackScreen;

  class SequenceTrackScreenInstance : public SequenceTrackInstanceBase<SequenceTrackScreenInstance, SequenceTrackScreenEvent>,
                                      public IScreen {
    public:
    SequenceTrackScreenInstance(SequenceTrackScreen& parent, SequenceInstance& sequenceInstance);

    void positionChanged();
    void eventTriggered(SequenceTrackScreenEvent& event) {}
    void eventTriggeredFrom(SequenceTrackScreenEvent& event) {}
    void resetInstance() {}

    /*************************************\
     * Implements ISequenceTrackInstance *
    \*************************************/
    void registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) override;

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    const IFloat* getYaw() const override;
    const IFloat* getPitch() const override;

    /*********************************\
     * Implements IAsset via IScreen *
    \*********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    SequenceTrackScreen& cParent;

    // Runtime data.
    IScreen* cRuntimeScreen;

    // Internal functions.
    void updateScreen();
    IScreen* getCurrentScreen() const;
  };
}

