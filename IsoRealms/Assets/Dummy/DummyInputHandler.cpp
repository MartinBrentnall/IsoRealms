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
#include "DummyInputHandler.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyInputHandler::DummyInputHandler(IProject& project) {
    // Nothing to do.
  }

  bool DummyInputHandler::input(sf::Event& event) {
    return false;
  }

  void DummyInputHandler::resetInput() {
    // Nothing to do.
  }

  bool DummyInputHandler::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void DummyInputHandler::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> DummyInputHandler::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool DummyInputHandler::isDefaultConfiguration() const {
    return true;
  }
}
