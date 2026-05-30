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

#include <set>
#include <queue>

#include "IsoRealms.h"

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
    static const std::string TYPE_DIGITAL_TO_ANALOGUE;

    DigitalToAnalogueMapping(Basics& basics, IResourceData& data, JSONObject object);

    DigitalToAnalogueMapping(const Metadata& metadata, IResourceData& owner);
    DigitalToAnalogueMapping(const Metadata& metadata, IResourceData& owner, JSONObject object);

    /************************************\
     * Implements IAnalogueInputMapping *
    \************************************/
    float getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    void save(JSONObject object, const std::string& name) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;
    void loadCustomMapping(JSONObject object) override;
    void registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) override;

    /**********************\
     * Implements IAsset *
    \**********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_DIGITAL_TO_ANALOGUE;
    static const std::string JSON_NAME;
    static const std::string JSON_TO_VALUE;
    static const std::string JSON_TYPE;

    // Definition data.
    DigitalInput cDefInput; /// Digital input to be converted.
    float cDefOutputValue;  /// Output value when the digital input is on (otherwise 0.0f).
  };
}
