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

#include <map>

#include "IDigitalInputMapping.h"
#include "HatHandler.h"

namespace IsoRealms {

  /**
   * A digital input mapping to an analogue stick or similar.
   */
  class HatMapping : public IDigitalInputMapping {
    public:
    static const std::string TYPE_HAT;

    /**
     * Construct a digital input mapping with the specified properties..
     *
     * @param hat The hat of the mapping.
     * @param positive If true, then state is true when input is above the
     *                 threshold, otherwise this is reversed.
     * @param threshold The threshold above or below which the state is true.
     * @throws ArgumentException If the specified direction is not known.
     */
    HatMapping(HatHandler& hatHandler, const HatHandler::Direction direction);

    /**
     * Construct a digital input mapping by loading the hat properties
     * from the specified node.
     *
     * @param node The node from which to read the properties.
     * @throws ArgumentException If the specified direction is not known.
     */
    HatMapping(HatHandler& hatHandler, JSONObject object);

    /**
     * Retrieve the direction of the specified name.  The key is always such that it
     * may be passed into to the getName() function in order to retrieve the
     * same name as was passed into this function.
     *
     * @param name Name of the direction to retrieve.
     * @return The direction of the specified name.
     * @throws ArgumentException If the specified name is not known.
     */
    static HatHandler::Direction getDirection(const std::string& name);

    /**
     * Retrieve the name of the specified direction.  The name is always such that it
     * may be passed into to the getKey() function in order to retrieve the
     * same direction as was passed into this function.
     *
     * @param key The direction to retrieve the name of.
     * @return Name of the specified direction.
     * @throws ArgumentException If the specified value is not known.
     */
    static std::string getName(const HatHandler::Direction value);

    /***********************************\
     * Implements IDigitalInputMapping *
    \***********************************/
    bool getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    void save(JSONObject object) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;

    private:
    static const std::string JSON_DIRECTION;
    static const std::string JSON_TYPE;

    static const std::map<std::string, HatHandler::Direction> cDirectionsByName;

    HatHandler& cHatHandler;

    const HatHandler::Direction cDirection; /// Determines the direction of true and false state.
  };
}
