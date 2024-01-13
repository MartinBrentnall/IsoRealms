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

#include "IDigitalInputMapping.h"

namespace IsoRealms {

  /**
   * A digital input mapping to a button on a controller.
   */
  class ButtonMapping : public IDigitalInputMapping {
    private:
    static const std::string TAG_BUTTON_DOWN;
    static const std::string TAG_BUTTON_UP;
  
    static const std::string ATTRIBUTE_BUTTON_VALUE;

    const unsigned int cButton; /// The key associated with this mapping.
    // TODO: Support inversion.
    // TODO: Support multiple controllers.
    
    public:
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
    ButtonMapping(DOMNode& node);
      
    /***********************************\
     * Implements IDigitalInputMapping *
    \***********************************/
    bool getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    void save(DOMNodeWriter* node) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;
  };
}
