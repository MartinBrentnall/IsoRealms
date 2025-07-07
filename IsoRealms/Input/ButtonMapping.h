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

#include "IDigitalInputMapping.h"

namespace IsoRealms {

  /**
   * A digital input mapping to a button on a controller.
   */
  class ButtonMapping : public IDigitalInputMapping {
    private:
    static const std::string JSON_BUTTON;
    static const std::string JSON_TYPE;

    const unsigned int cButton; /// The key associated with this mapping.
    // TODO: Support inversion.
    // TODO: Support multiple controllers.

    public:
    static const std::string TYPE_BUTTON_DOWN;

    static const unsigned int CROSS;
    static const unsigned int CIRCLE;
    static const unsigned int SQUARE;
    static const unsigned int TRIANGLE;
    static const unsigned int L2;

    /**
     * Construct a digital input mapping associated with the specified button.
     * 
     * @param key The button to associate this digital input mapping with.
     */
    ButtonMapping(const unsigned int button);
    
    /**
     * Construct a digital input mapping by loading the associated button from
     * the specified node.
     * 
     * @param node The node from which to read the associated button.
     */
    ButtonMapping(JSONObject object);

    /***********************************\
     * Implements IDigitalInputMapping *
    \***********************************/
    bool getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    void save(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getProperties() override;
    std::string getShortName() const override;
    std::string getLongName() const override;
  };
}
