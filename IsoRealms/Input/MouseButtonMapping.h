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

#include <SFML/Window/Mouse.hpp>

#include "IDigitalInputMapping.h"

namespace IsoRealms {

  /**
   * A digital input mapping to a mouse button.
   */
  class MouseButtonMapping : public IDigitalInputMapping {
    private:
    static const std::string JSON_BUTTON;
    static const std::string JSON_TYPE;

    static const std::string UNMAPPED_BUTTON_PREFIX;

    static const std::map<std::string, sf::Mouse::Button> cButtonsByName; /// Mapping of buttons by name.

    const sf::Mouse::Button cButton; /// The key associated with this mapping.
    // TODO: Support inversion.

    public:
    static const std::string TYPE_MOUSE_BUTTON_DOWN;

    static sf::Mouse::Button getButton(const std::string& name);
    static std::string getName(const sf::Mouse::Button button);

    /**
     * Construct a digital input mapping associated with the specified key.
     *
     * @param key The key to associate this digital input mapping with.
     */
    MouseButtonMapping(const sf::Mouse::Button button);

    /**
     * Construct a digital input mapping by loading the associated key from the
     * specified node.
     *
     * @param node The node from which to read the associated key.
     */
    MouseButtonMapping(JSONObject object);

    /***********************************\
     * Implements IDigitalInputMapping *
    \***********************************/
    bool matches(const sf::Event& event) const override;
    bool getState(const sf::Event& event) const override;
    void save(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getProperties() override;
    std::string getShortName() const override;
    std::string getLongName() const override;
  };
}

