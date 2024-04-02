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
#include <queue>

#include "IsoRealms/Exception/ActionException.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/Input.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

#include "Modules/Basics/DigitalInput/DigitalInput.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * An analogue input mapping that wraps a digital input and outputs a
   * specified analogue value if the digital input is active (otherwise it
   * outputs 0.0f).
   */
  class DigitalToAnalogueMapping final : public IAnalogueInputMapping {
    public:
    DigitalToAnalogueMapping(IProject* project, DOMNode& node);
    
    /************************************\
     * Implements IAnalogueInputMapping *
    \************************************/
    float getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    void save(DOMNodeWriter* node, const std::string& name) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;
    void loadCustomMapping(DOMNode& node) override;
    void registerAssets(IAssetRegistry* assets) override;
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser) override;
    
    private:

    // DOM strings.
    static const std::string ATTRIBUTE_TO_VALUE;

    // Definition data.
    DigitalInput cDefInput; /// Digital input to be converted.
    float cDefOutputValue;  /// Output value when the digital input is on (otherwise 0.0f).
  };
}
