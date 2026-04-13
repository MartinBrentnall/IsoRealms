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
#include "EditableRegistry.h"

namespace IsoRealms {
  EditableRegistry::EditableRegistry() :
            AssetClientManager(&cLiteral) {
  }

  IEditableScreen* EditableRegistry::Dummy::createEditableScreen(Project* project, IDialogManager& dialogManager) {
    return &DUMMY;
  }

  bool EditableRegistry::Dummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void EditableRegistry::Dummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void EditableRegistry::Dummy::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool EditableRegistry::Dummy::isDefaultConfiguration() const {
    return true;
  }

  void EditableRegistry::Dummy::Instance::notifyVisible() {
    // Nothing to do.
  }

  void EditableRegistry::Dummy::Instance::notifyHidden() {
    // Nothing to do.
  }

  void EditableRegistry::Dummy::Instance::notifyLostFocus() {
    // Nothing to do.
  }

  void EditableRegistry::Dummy::Instance::notifyGainedFocus() {
    // Nothing to do.
  }

  std::vector<std::string> EditableRegistry::Dummy::Instance::getDigitalInputs() const {
    return std::vector<std::string>();
  }

  std::vector<std::string> EditableRegistry::Dummy::Instance::getAnalogueInputs() const {
    return std::vector<std::string>();
  }

  void EditableRegistry::Dummy::Instance::setDigitalInput(const std::string& name, IBoolean* input) {
    // Nothing to do.
  }

  void EditableRegistry::Dummy::Instance::setAnalogueInput(const std::string& name, IFloat* input) {
    // Nothing to do.
  }

  void EditableRegistry::Dummy::Instance::setExitAction(IAction* action) {
    // Nothing to do.
  }

  void EditableRegistry::Dummy::Instance::setAppearance(IFont* font, float scale) {
    // Nothing to do.
  }

  IScreen* EditableRegistry::Dummy::Instance::screen() {
    return nullptr;
  }

  void EditableRegistry::Dummy::Instance::renderScreen(float scale, float aspectRatio) const {
    // Nothing to do.
  }

  const IFloat* EditableRegistry::Dummy::Instance::getYaw() const {
    return nullptr;
  }

  const IFloat* EditableRegistry::Dummy::Instance::getPitch() const {
    return nullptr;
  }

  bool EditableRegistry::Dummy::Instance::input(sf::Event& event) {
    return false;
  }

  void EditableRegistry::Dummy::Instance::resetInput() {
    // Nothing to do.
  }

  bool EditableRegistry::Dummy::Instance::renderAssetIcon() const {
    return false;
  }

  void EditableRegistry::Dummy::Instance::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void EditableRegistry::Dummy::Instance::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool EditableRegistry::Dummy::Instance::isDefaultConfiguration() const {
    return true;
  }

  EditableRegistry::Dummy::Instance EditableRegistry::Dummy::DUMMY;
}
