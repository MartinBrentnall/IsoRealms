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
#include "LayoutLocation.h"

#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  LayoutLocation::LayoutLocation(UI& ui, LayoutComponentEdge& owner) :
            Asset<LayoutLocation, ILayoutLocation, UI>(ui, ui.getLayoutLocation(this, "Absolute", owner)),
            cOwner(owner) {
  }

  ILayoutLocation* LayoutLocation::createLiteralAsset(UI& ui) {
    return ui.createLiteralLayoutLocation(this, cOwner);
  }
  
  ILayoutLocation* LayoutLocation::getAsset(UI& ui, JSONObject object) {
    return ui.getLayoutLocation(this, object, cOwner);
  }
  
  ILayoutLocation* LayoutLocation::getAsset(UI& ui, const std::string& id) {
    return ui.getLayoutLocation(this, id, cOwner);
  }
  
  bool LayoutLocation::isDefaultConfiguration() const {
    return true;
  }
}
