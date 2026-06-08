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
#include "GameControllerButton.h"

#include <string>

#include "IsoRealms/Editing.h"
#include "IsoRealms/Editing/Property/IPropertyMaker.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  GameControllerButton::ButtonChooser::ButtonChooser(const Metadata& metadata) :
          cMetadata(metadata) {
  }

  GameControllerButton::GameControllerButton(const Metadata& metadata, IResourceData& owner) :
          cMetadata(metadata),
          cButtonChooser(metadata),
          cButton(0) {
  }

  GameControllerButton::GameControllerButton(const Metadata& metadata, IResourceData& owner, JSONObject object) :
          GameControllerButton(metadata, owner) {
    cButton = object.getInteger(JSON_BUTTON);
  }

  bool GameControllerButton::matches(const sf::Event& event) const {
    return (event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased) && event.joystickButton.button == cButton;
  }

  bool GameControllerButton::getState(const sf::Event& event) const {
    return event.type == sf::Event::JoystickButtonPressed;
  }

  std::string GameControllerButton::getShortName() const {
    return "B" + Utils::toString(cButton);
  }
    
  std::string GameControllerButton::getLongName() const {
    return "Button " + Utils::toString(cButton);
  }

  std::string GameControllerButton::getLocalizedName() const {
    return getChoiceLabel(cMetadata, cButton);
  }

  bool GameControllerButton::renderAssetIcon() const {
    return false;
  }

  void GameControllerButton::saveAsset(JSONObject object) const {
    object.addInteger(JSON_BUTTON, cButton);
  }

  void GameControllerButton::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyOptional("Button", cButtonChooser, "", []() {
      return true;
    }, [this](const std::string& button) {
      cButton = static_cast<unsigned int>(std::stoul(button.substr(1)));
    }, [this]() {
      return getLocalizedName();
    });
  }

  bool GameControllerButton::isDefaultConfiguration() const {
    return true;
  }

  void GameControllerButton::ButtonChooser::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    for (unsigned int i = 0; i < sf::Joystick::ButtonCount; i++) {
      const std::string mID = "B" + Utils::toString(i);
      getTreeItemInfoFunction(TreeItemInfo{mID, GameControllerButton::getChoiceLabel(cMetadata, i)});
    }
  }

  std::string GameControllerButton::getChoiceLabel(const Metadata& metadata, unsigned int button) {
    const std::string mLabelTemplate = metadata.getPropertyData("ButtonChoice").getName();
    const std::string mIndex = Utils::toString(button);
    const std::string::size_type mPlaceholder = mLabelTemplate.find("%1");
    if (mPlaceholder != std::string::npos) {
      return mLabelTemplate.substr(0, mPlaceholder) + mIndex + mLabelTemplate.substr(mPlaceholder + 2);
    }
    return mLabelTemplate + mIndex;
  }
}
