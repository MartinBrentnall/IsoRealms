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

#include "ColourTrackEventFade.h"
#include "ColourTrackEventPause.h"
#include "IColourTrackEvent.h"
#include "ISequenceTrack.h"

namespace IsoRealms::Basics {

  /**
   * Colour track fades between colours over various durations.
   */
  class ColourTrack final : public ISequenceTrack,
                            public IColour {
    public:
    static const std::string TYPE_NAME;

    ColourTrack(DOMNode& node, IProject* project);

    /*****************************\
     * Implements ISequenceTrack *
    \*****************************/
    void registerAssets(IAssetRegistry* assets) override;
    void unregisterAssets(IAssetRemover* assets) override;
    void save(DOMNodeWriter* node) const override;
    bool play(unsigned int milliseconds) override;
    void reset() override;
    unsigned int getDuration() const override;
    void render(float left, float bottom, float right, float top) const override;

    /**********************\
     * Implements IColour *
    \**********************/
    void set() const override;
    float getRed() const override;
    float getGreen() const override;
    float getBlue() const override;
    float getAlpha() const override;

    private:

    // DOM strings.
    static const std::string TAG_INIT;

    static const std::string ATTRIBUTE_DURATION;
    static const std::string ATTRIBUTE_OUTPUT;

    // Definition data.
    std::string cDefName;
    Colour cDefInitColour;
    std::vector<std::unique_ptr<IColourTrackEvent>> cDefEvents;

    // Runtime data.
    LiteralColour cRuntimeColour;
    unsigned int cRuntimeEvent;
    int cRuntimeEventPosition;

    // Misc.
    IStateNotifier<IColour>* cStateNotifier;

    /**********************\
     * Internal Functions *
    \**********************/
    void updateColour();
    const IColour* getPreviousColour();
    void stateChanged(IColour* colour);
  };
}
