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

#include "IsoRealms/IComponentDefiner.h"
#include "IsoRealms/Project/Options.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  GameControllerButton::GameControllerButton(IComponentData& owner) {
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
    return getChoiceLabel(cButton);
  }

  void GameControllerButton::defineResource(IComponentDefiner& definer) {
    Options mNoEdit;
    mNoEdit.addOption(Options::PROPERTY_NO_EDIT, "true");
    Options mNoPersist;
    mNoPersist.addOption(Options::PROPERTY_NO_PERSIST, "true");
    definer.propertyInteger("button", [this]() {return static_cast<int>(cButton);}, [this](int button) {cButton = static_cast<unsigned int>(button);}, 0, [](int) {return true;}, nullptr, mNoEdit);
    definer.propertyOptional("button", cButtonChooser, "", []() {
      return true;
    }, [this](const std::string& button) {
      cButton = static_cast<unsigned int>(std::stoul(button.substr(1)));
    }, [this]() {
      return getLocalizedName();
    }, mNoPersist);
  }

  void GameControllerButton::ButtonChooser::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    for (unsigned int i = 0; i < sf::Joystick::ButtonCount; i++) {
      const std::string mID = "B" + Utils::toString(i);
      getTreeItemInfoFunction(TreeItemInfo{mID, GameControllerButton::getChoiceLabel(i)});
    }
  }

  // TODO: Support localized names.
  std::string GameControllerButton::getChoiceLabel(unsigned int button) {
    return "B" + Utils::toString(button);
    // const std::string mLabelTemplate = metadata.getPropertyData("ButtonChoice").getName();
    // const std::string mIndex = Utils::toString(button);
    // const std::string::size_type mPlaceholder = mLabelTemplate.find("%1");
    // if (mPlaceholder != std::string::npos) {
    //   return mLabelTemplate.substr(0, mPlaceholder) + mIndex + mLabelTemplate.substr(mPlaceholder + 2);
    // }
    // return mLabelTemplate + mIndex;
  }
}
