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

#include "IsoRealms/Utils.h"

#include "IsoRealms/Assets/Type/IAnalogueInputMapping.h"

namespace IsoRealms {
  class Metadata;
  class IResourceData;

  /**
   * A analogue input mapping to an analogue stick or similar.
   */
  class AxisMapping : public IAnalogueInputMapping {
    public:

    /**
     * Construct a analogue input mapping with the specified properties..
     * 
     * @param axis The axis of the mapping.
     * @param positive If true, then state is true when input is above the
     *                 threshold, otherwise this is reversed.
     * @param threshold The threshold above or below which the state is true.
     */
    AxisMapping(const unsigned int axis, const bool positive, const int threshold);
    
    AxisMapping(const Metadata& metadata, IResourceData& owner);
    AxisMapping(const Metadata& metadata, IResourceData& owner, JSONObject object);

    /**
     * Construct a analogue input mapping by loading the axis properties
     * from the specified node.
     * 
     * @param node The node from which to read the properties.
     */
    AxisMapping(JSONObject object);

    /************************************\
     * Implements IAnalogueInputMapping *
    \************************************/
    std::string getName() const override;
    float getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;
    void loadCustomMapping(JSONObject object) override;
    void registerAssets(ResourceAssetRegistry& assets) override;

    /***********************************************\
     * Implements IAsset via IAnalogueInputMapping *
    \***********************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    inline static const std::string JSON_AXIS      = "axis";
    inline static const std::string JSON_DEAD_ZONE = "deadZone";

    // Definition data.
    unsigned int cDefAxis; /// Axis of this mapping.
    float cDefDeadZone;    /// Dead zone of this mapping
  };
}
