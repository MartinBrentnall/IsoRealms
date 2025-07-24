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

#include <memory>
#include <string>
#include <vector>

#include <SFML/Window/Event.hpp>

#include "IsoRealms/Persistence.h"

namespace IsoRealms {
  class PropertyMaker;
  class IProperty;
  
  /**
   * Interface to a digital input mapping on which input events can be tested
   * and processed.
   */
  class IDigitalInputMapping {
    public:
      
    /**
     * Determine whether the specified event is a match for this digital input
     * mapping (i.e. an event that could cause a state change within this
     * mapping).  For example, the event of pressing or releasing the key to
     * which this input is mapped would be regarded as a match.
     * 
     * @param event The event to test for a match to this mapping.
     * @return true if the event is a match to this mapping, otherwise false
     *         if the event is not a match to this mapping.
     */
    virtual bool matches(const sf::Event& event) const = 0;
    
    /**
     * Get the current state of this digital input mapping according to the
     * specified event.  Note that the behaviour of this function is not
     * defined in case the event does not match this input mapping.
     * 
     * @param event The event to find the state of.
     * @return The state of this digital input mapping.
     */
    virtual bool getState(const sf::Event& event) const = 0;
    
    /**
     * Save this digital input mapping to the specified node.
     * 
     * @param node The node to save this digital input mapping to.
     */
    virtual void save(JSONObject object) const = 0;

    virtual void getProperties(PropertyMaker& owner) = 0;

    /**
     * Return a short name of this digital input mapping.  The short name is
     * intended only for presentation purposes to the user.
     * 
     * @return Short name of this digital input mapping.
     */
    virtual std::string getShortName() const = 0;
    
    /**
     * Return a long name of this digital input mapping.  The long name is
     * intended only for presentation purposes to the user.
     * 
     * @return Long name of this digital input mapping.
     */
    virtual std::string getLongName() const = 0;

    virtual ~IDigitalInputMapping() {
    }
  };
}
