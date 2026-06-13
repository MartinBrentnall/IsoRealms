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
#include "MouseButton.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/Editing/Property/IPropertyMaker.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  sf::Mouse::Button MouseButton::getButton(const std::string& name) {
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

  std::string MouseButton::getName(const sf::Mouse::Button button) {
    for (std::map<std::string, sf::Mouse::Button>::const_iterator i = cButtonsByName.begin(); i != cButtonsByName.end(); i++) {
      if (i->second == button) {
        return i->first;
      }
    }

    // Handle buttons that aren't in the name map.
    return UNMAPPED_BUTTON_PREFIX + Utils::toString(button);
  }

  MouseButton::ButtonChooser::ButtonChooser(const Metadata& metadata) :
          cMetadata(metadata) {
  }

  MouseButton::MouseButton(const Metadata& metadata, IComponentData& owner) :
          cMetadata(metadata),
          cButtonChooser(metadata),
          cButton(sf::Mouse::Left) {
  }

  MouseButton::MouseButton(const Metadata& metadata, IComponentData& owner, JSONObject object) :
          MouseButton(metadata, owner) {
    cButton = getButton(object.getString(JSON_BUTTON));
  }

  bool MouseButton::matches(const sf::Event& event) const {
    return (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) && event.mouseButton.button == cButton;
  }

  bool MouseButton::getState(const sf::Event& event) const {
    return event.type == sf::Event::MouseButtonPressed;
  }

  std::string MouseButton::getShortName() const {
    return getName(cButton);
  }

  std::string MouseButton::getLongName() const {
    return getName(cButton) + " Mouse Button";
  }

  std::string MouseButton::getLocalizedName() const {
    const std::string mName = getShortName();
    return cButtonsByName.find(mName) != cButtonsByName.end() ? cMetadata.getPropertyData(mName).getName() : mName;
  }

  bool MouseButton::renderAssetIcon() const {
    return false;
  }

  void MouseButton::saveAsset(JSONObject object) const {
    object.addString(JSON_BUTTON, getShortName());
  }

  void MouseButton::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyOptional(JSON_BUTTON, cButtonChooser, "", []() {
      return true;
    }, [this](const std::string& button) {
      cButton = getButton(button);
    }, [this]() {
      return getLocalizedName();
    });
  }

  bool MouseButton::isDefaultConfiguration() const {
    return true;
  }
  
  void MouseButton::ButtonChooser::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    for (std::map<std::string, sf::Mouse::Button>::const_iterator i = cButtonsByName.begin(); i != cButtonsByName.end(); i++) {
      getTreeItemInfoFunction(TreeItemInfo{i->first, cMetadata.getPropertyData(i->first).getName()});
    }
  }
}
