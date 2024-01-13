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

#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "ISequenceTrack.h"

namespace IsoRealms::Basics {

  /**
   * Action track executes actions at predetermined times in a Sequence.
   */
  class ActionTrack final : public ISequenceTrack {
    public:
    static const std::string TYPE_NAME;

    ActionTrack(DOMNode& node, IProject* project);

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

    private:

    // DOM strings.
    static const std::string TAG_ACTION;
    
    static const std::string ATTRIBUTE_DELAY;
      
    // Definition data.
    class ActionEvent final {
      public:
      ActionEvent(DOMNode& node, IProject* project);

      void save(DOMNodeWriter* node) const;
      unsigned int getDuration() const;
      void execute();
      
      private:
      Action cDefAction;
      unsigned int cDefDuration;
    };
    std::vector<std::unique_ptr<ActionEvent>> cDefEvents;

    // Runtime data.
    unsigned int cRuntimeAction;
    unsigned int cRuntimeActionRemaining;
  };
}
