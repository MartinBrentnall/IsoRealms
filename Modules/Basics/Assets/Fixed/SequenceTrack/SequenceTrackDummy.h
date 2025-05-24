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

#include <set>

#include "IsoRealms/Literals.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "Modules/Basics/Assets/Type/ISequenceTrack.h"

namespace IsoRealms::Basics {
  class Sequence;

  /**
   * Colour track fades between colours over various durations.
   */
  class SequenceTrackDummy final : public ISequenceTrack {
    public:
    SequenceTrackDummy(Sequence& sequence);

    /*****************************\
     * Implements ISequenceTrack *
    \*****************************/
    void registerAssets(IAssetRegistry& assets) override;
    void unregisterAssets(IAssetRemover& assets, bool relinquish) override;
    bool play(unsigned int milliseconds) override;
    void reset() override;
    unsigned int getDuration() const override;
    void setName(const std::string& name) override;
    std::string getName() const override;
    ISequenceTrackEvent* createEvent(IProject& project, unsigned int time) override;
    void deleteEvent(ISequenceTrackEvent* event) override;
    std::vector<ISequenceTrackEvent*> getEvents() override;
    void stopPreview() override;
    void setPreviewPosition(long position) override;
    void renderIcon() const override;
    void render(float left, float bottom, float right, float top, double startTime, double endTime) const override;

    /****************************************\
     * Implements IAsset via ISequenceTrack *
    \****************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;
  };
}
