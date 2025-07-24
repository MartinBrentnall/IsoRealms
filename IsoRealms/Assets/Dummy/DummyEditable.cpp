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
#include "DummyEditable.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyEditable::EditableScreen DummyEditable::DUMMY;

  DummyEditable::DummyEditable(IResourceData& owner) {
    // Nothing to do.
  }

  IEditableScreen* DummyEditable::createEditableScreen(Project* project, IDialogManager& dialogManager) {
    return &DUMMY;
  }

  bool DummyEditable::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void DummyEditable::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void DummyEditable::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool DummyEditable::isDefaultConfiguration() const {
    return true;
  }

  void DummyEditable::EditableScreen::notifyVisible() {
    // Nothing to do.
  }

  void DummyEditable::EditableScreen::notifyHidden() {
    // Nothing to do.
  }

  void DummyEditable::EditableScreen::notifyLostFocus() {
    // Nothing to do.
  }

  void DummyEditable::EditableScreen::notifyGainedFocus() {
    // Nothing to do.
  }

  std::vector<std::string> DummyEditable::EditableScreen::getDigitalInputs() const {
    return std::vector<std::string>();
  }

  std::vector<std::string> DummyEditable::EditableScreen::getAnalogueInputs() const {
    return std::vector<std::string>();
  }

  void DummyEditable::EditableScreen::setDigitalInput(const std::string& name, IBoolean* input) {
    // Nothing to do.
  }

  void DummyEditable::EditableScreen::setAnalogueInput(const std::string& name, IFloat* input) {
    // Nothing to do.
  }

  void DummyEditable::EditableScreen::setExitAction(IAction* action) {
    // Nothing to do.
  }

  void DummyEditable::EditableScreen::setAppearance(IFont* font, float scale) {
    // Nothing to do.
  }

  IScreen* DummyEditable::EditableScreen::screen() {
    return nullptr;
  }

  void DummyEditable::EditableScreen::renderScreen(float scale, float aspectRatio) const {
    // Nothing to do.
  }

  const IFloat* DummyEditable::EditableScreen::getYaw() const {
    return nullptr;
  }

  const IFloat* DummyEditable::EditableScreen::getPitch() const {
    return nullptr;
  }

  bool DummyEditable::EditableScreen::input(sf::Event& event) {
    return false;
  }

  void DummyEditable::EditableScreen::resetInput() {
    // Nothing to do.
  }

  bool DummyEditable::EditableScreen::renderAssetIcon() const {
    return false;
  }

  void DummyEditable::EditableScreen::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void DummyEditable::EditableScreen::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool DummyEditable::EditableScreen::isDefaultConfiguration() const {
    return true;
  }
}
