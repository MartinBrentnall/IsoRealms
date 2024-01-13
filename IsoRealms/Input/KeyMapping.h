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

#include <SFML/Window/Keyboard.hpp>

#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"

#include "IDigitalInputMapping.h"

namespace IsoRealms {
  
  /**
   * A digital input mapping to a key on a keyboard.
   */
  class KeyMapping : public IDigitalInputMapping {
    private:
    static const std::string TAG_KEY_DOWN;
    static const std::string TAG_KEY_UP;
    
    static const std::string ATTRIBUTE_KEY_VALUE;

    static const std::string UNMAPPED_KEY_PREFIX;
      
    static const std::map<std::string, sf::Keyboard::Key> cKeysByName; /// Mapping of keys by name.

    const sf::Keyboard::Key cKey; /// The key associated with this mapping.
    // TODO: Support inversion.
    
    public:
      
    /**
     * Retrieve the key of the specified name.  The key is always such that it
     * may be passed into to the getName() function in order to retrieve the
     * same name as was passed into this function.
     * 
     * @param name Name of the key to retrieve.
     * @return The key of the specified name.
     * @throws ArgumentException If the specified key name is not known.
     */
    static sf::Keyboard::Key getKey(const std::string& name);
    
    /**
     * Retrieve the name of the specified key.  The name is always such that it
     * may be passed into to the getKey() function in order to retrieve the
     * same key as was passed into this function.
     * 
     * @param key The key to retrieve the name of.
     * @return Name of the specified key.
     */
    static std::string getName(const sf::Keyboard::Key& key);
    
    /**
     * Construct a digital input mapping associated with the specified key.
     * 
     * @param key The key to associate this digital input mapping with.
     */
    KeyMapping(const sf::Keyboard::Key key);
    
    /**
     * Construct a digital input mapping by loading the associated key from the
     * specified node.
     * 
     * @param node The node from which to read the associated key.
     */
    KeyMapping(DOMNode& node);

    /***********************************\
     * Implements IDigitalInputMapping *
    \***********************************/
    bool matches(const sf::Event& event) const override;
    bool getState(const sf::Event& event) const override;
    void save(DOMNodeWriter* node) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;
  };
}
