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
#include "MouseButtonMapping.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  const std::string MouseButtonMapping::JSON_BUTTON = "button";
  const std::string MouseButtonMapping::JSON_TYPE   = "type";

  const std::string MouseButtonMapping::TYPE_MOUSE_BUTTON_DOWN = "MouseButtonDown";

  const std::string MouseButtonMapping::UNMAPPED_BUTTON_PREFIX = "Mouse Button ";

  const std::map<std::string, sf::Mouse::Button> MouseButtonMapping::cButtonsByName = {
    {"Left",         sf::Mouse::Button::Left},
    {"Right",        sf::Mouse::Button::Right},
    {"Middle",       sf::Mouse::Button::Middle},
    {"ExtraButton1", sf::Mouse::Button::XButton1},
    {"ExtraButton2", sf::Mouse::Button::XButton2}
  };

  sf::Mouse::Button MouseButtonMapping::getButton(const std::string& name) {
    std::map<std::string, sf::Mouse::Button>::const_iterator i = cButtonsByName.find(name);
    if (i == cButtonsByName.end()) {

      // Handle buttons that aren't in the name map.
      if (name.compare(0, UNMAPPED_BUTTON_PREFIX.length(), UNMAPPED_BUTTON_PREFIX) == 0) {
        std::stringstream mStream(name);
        int mCode;
        mStream >> mCode;
        sf::Mouse::Button mButtonCode = static_cast<sf::Mouse::Button>(mCode);
        return mButtonCode;
      }
      throw ArgumentException("ERROR: MouseButtonMapping::getButton: Button of name \"" + name + "\" not known.");
    }
    return i->second;
  }

  std::string MouseButtonMapping::getName(const sf::Mouse::Button button) {
    for (std::map<std::string, sf::Mouse::Button>::const_iterator i = cButtonsByName.begin(); i != cButtonsByName.end(); i++) {
      if (i->second == button) {
        return i->first;
      }
    }

    // Handle buttons that aren't in the name map.
    return UNMAPPED_BUTTON_PREFIX + Utils::toString(button);
  }

  MouseButtonMapping::MouseButtonMapping(sf::Mouse::Button button) :
          cButton(button) {
  }

  MouseButtonMapping::MouseButtonMapping(JSONObject object) :
          cButton(getButton(object.getString(JSON_BUTTON))) {
  }

  bool MouseButtonMapping::matches(const sf::Event& event) const {
    return (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) && event.mouseButton.button == cButton;
  }

  bool MouseButtonMapping::getState(const sf::Event& event) const {
    return event.type == sf::Event::MouseButtonPressed;
  }

  void MouseButtonMapping::save(JSONObject object) const {
    object.addString(JSON_TYPE, TYPE_MOUSE_BUTTON_DOWN);
    object.addString(JSON_BUTTON, getShortName());
  }

  void MouseButtonMapping::getProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  std::string MouseButtonMapping::getShortName() const {
    return getName(cButton);
  }

  std::string MouseButtonMapping::getLongName() const {
    return getName(cButton) + " Mouse Button";
  }
}

