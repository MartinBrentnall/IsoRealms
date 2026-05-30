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
#include "ButtonMapping.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
#if _WIN32
  const unsigned int ButtonMapping::CROSS    = 1;
  const unsigned int ButtonMapping::CIRCLE   = 2;
  const unsigned int ButtonMapping::SQUARE   = 0;
  const unsigned int ButtonMapping::TRIANGLE = 3;
#else
  const unsigned int ButtonMapping::CROSS    = 0;
  const unsigned int ButtonMapping::CIRCLE   = 1;
  const unsigned int ButtonMapping::SQUARE   = 3;
  const unsigned int ButtonMapping::TRIANGLE = 2;
#endif
  const unsigned int ButtonMapping::L2       = 6;

  ButtonMapping::ButtonMapping(const unsigned int button) :
          cButton(button) {
  }

  ButtonMapping::ButtonMapping(const Metadata& metadata, IResourceData& owner) {
    // TODO: Implement this.
  }

  ButtonMapping::ButtonMapping(const Metadata& metadata, IResourceData& owner, JSONObject object) :
          ButtonMapping(metadata, owner) {
    cButton = object.getInteger(JSON_BUTTON);
  }

  bool ButtonMapping::matches(const sf::Event& event) const {
    return (event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased) && event.joystickButton.button == cButton;
  }

  bool ButtonMapping::getState(const sf::Event& event) const {
    return event.type == sf::Event::JoystickButtonPressed;
  }

  std::string ButtonMapping::getShortName() const {
    return "B" + Utils::toString(cButton);
  }
    
  std::string ButtonMapping::getLongName() const {
    return "Button " + Utils::toString(cButton);
  }

  bool ButtonMapping::renderAssetIcon() const {
    return false;
  }

  void ButtonMapping::saveAsset(JSONObject object) const {
    object.addInteger(JSON_BUTTON, cButton);
  }

  void ButtonMapping::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool ButtonMapping::isDefaultConfiguration() const {
    return true;
  }
}
