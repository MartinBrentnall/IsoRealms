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
#include "LayoutLocation.h"

#include "IsoRealms/Editing/Property/IProperty.h"

#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  LayoutLocation::LayoutLocation(IUI& ui, LayoutComponentEdge& owner) :
            Asset<LayoutLocation, ILayoutLocation, IUI>(ui, ui.getLayoutLocation(this, "Absolute", owner)),
            cOwner(owner) {
  }

  ILayoutLocation* LayoutLocation::createLiteralAsset(IUI& ui) {
    return ui.createLiteralLayoutLocation(this, cOwner);
  }
  
  ILayoutLocation* LayoutLocation::getAsset(IUI& ui, JSONObject object) {
    return ui.getLayoutLocation(this, object, cOwner);
  }
  
  ILayoutLocation* LayoutLocation::getAsset(IUI& ui, const std::string& id) {
    return ui.getLayoutLocation(this, id, cOwner);
  }
  
  std::vector<std::string> LayoutLocation::getAvailableProviders() const {
    return cManager.getAllLayoutLocations();
  }  

  bool LayoutLocation::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderLayoutLocationIcon(id);
  }

  bool LayoutLocation::hasConfiguration() const {
    return cManager.isLayoutLocationConfigurable(getID());
  }

  bool LayoutLocation::isDefaultConfiguration() const {
    return true;
  }
}
