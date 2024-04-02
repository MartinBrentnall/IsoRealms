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

#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/Utils.h"

#include "IAnalogueInputMapping.h"

namespace IsoRealms {

  /**
   * A analogue input mapping to an analogue stick or similar.
   */
  class AxisMapping : public IAnalogueInputMapping {
    private:
    static const std::string TAG_AXIS;
    
    static const std::string ATTRIBUTE_AXIS;
    static const std::string ATTRIBUTE_DEAD_ZONE;
  
    const unsigned int cDefAxis; /// Axis of this mapping.
    const float cDefDeadZone;    /// Dead zone of this mapping
    
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
    
    /**
     * Construct a analogue input mapping by loading the axis properties
     * from the specified node.
     * 
     * @param node The node from which to read the properties.
     */
    AxisMapping(DOMNode& node);
      
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
  };
}
