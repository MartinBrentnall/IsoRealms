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
#include "GameControllerAxis.h"

#include <string>

#include "IsoRealms/Editing/Property/IPropertyMaker.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Project/Options.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"

namespace IsoRealms {
  GameControllerAxis::AxisChooser::AxisChooser(const Metadata& metadata) :
          cMetadata(metadata) {
  }

  GameControllerAxis::GameControllerAxis(const Metadata& metadata, IComponentData& owner) :
          cMetadata(metadata),
          cAxisChooser(metadata),
          cDefAxis(0),
          cDefDeadZone(0.16f) {
  }

  GameControllerAxis::GameControllerAxis(const Metadata& metadata, IComponentData& owner, JSONObject object) :
          GameControllerAxis(metadata, owner) {
    cDefAxis = object.getInteger(JSON_AXIS);
    cDefDeadZone = object.getFloat(JSON_DEAD_ZONE);
  }

  std::string GameControllerAxis::getName() const {
    return ""; // TODO: Implement this.
  }

  bool GameControllerAxis::matches(const sf::Event& event) const {
    return event.type == sf::Event::JoystickMoved && event.joystickMove.axis == cDefAxis;
  }

  float GameControllerAxis::getState(const sf::Event& event) const {
    float mValue = event.joystickMove.position / 100.0f;
    return std::abs(mValue) < cDefDeadZone ? 0 : (mValue - (mValue < 0 ? -cDefDeadZone : cDefDeadZone)) * (1.0f / (1.0f - cDefDeadZone));
  }

  std::string GameControllerAxis::getShortName() const {
    return "A" + Utils::toString(cDefAxis);
  }
    
  std::string GameControllerAxis::getLongName() const {
    return "Axis " + Utils::toString(cDefAxis);
  }

  std::string GameControllerAxis::getLocalizedName() const {
    return getChoiceLabel(cMetadata, cDefAxis);
  }
  
  void GameControllerAxis::loadCustomMapping(JSONObject object) {
    // Nothing to do.
  }

  void GameControllerAxis::registerAssets(ComponentAssetRegistry& assets) {
    // Nothing to do.
  }

  bool GameControllerAxis::renderAssetIcon() const {
    return false;
  }

  void GameControllerAxis::saveAsset(JSONObject object) const {
    object.addInteger(JSON_AXIS, cDefAxis);
    object.addFloat(JSON_DEAD_ZONE, cDefDeadZone);
  }

  void GameControllerAxis::getAssetProperties(IPropertyMaker& owner) {
    Options mNoEdit;
    mNoEdit.addOption(Options::PROPERTY_NO_EDIT, "true");
    Options mNoPersist;
    mNoPersist.addOption(Options::PROPERTY_NO_PERSIST, "true");
    owner.createPropertyNativeInteger(JSON_AXIS, [this]() {return cDefAxis;}, [this](int axis) {cDefAxis = axis;}, 0, [](int) {return true;}, nullptr, mNoEdit);
    owner.createPropertyOptional(JSON_AXIS, cAxisChooser, "", []() {
      return true;
    }, [this](const std::string& axis) {
      cDefAxis = static_cast<unsigned int>(std::stoul(axis.substr(1)));
    }, [this]() {
      return getLocalizedName();
    }, mNoPersist);
    owner.createPropertyNativeFloat(JSON_DEAD_ZONE, [this]() {return cDefDeadZone;}, [this](float deadZone) {cDefDeadZone = deadZone;});
  }

  bool GameControllerAxis::isDefaultConfiguration() const {
    return cDefAxis == 0 && cDefDeadZone == 0.16f;
  }

  void GameControllerAxis::AxisChooser::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    for (unsigned int i = 0; i < sf::Joystick::AxisCount; i++) {
      const std::string mID = "A" + Utils::toString(i);
      getTreeItemInfoFunction(TreeItemInfo{mID, GameControllerAxis::getChoiceLabel(cMetadata, i)});
    }
  }

  std::string GameControllerAxis::getChoiceLabel(const Metadata& metadata, unsigned int axis) {
    const std::string mLabelTemplate = metadata.getPropertyData("AxisChoice").getName();
    const std::string mIndex = Utils::toString(axis);
    const std::string::size_type mPlaceholder = mLabelTemplate.find("%1");
    if (mPlaceholder != std::string::npos) {
      return mLabelTemplate.substr(0, mPlaceholder) + mIndex + mLabelTemplate.substr(mPlaceholder + 2);
    }
    return mLabelTemplate + mIndex;
  }
}
