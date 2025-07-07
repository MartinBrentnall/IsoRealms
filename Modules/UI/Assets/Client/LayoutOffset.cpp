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
#include "LayoutOffset.h"

#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  LayoutOffset::LayoutOffset(UI& ui, LayoutComponentEdge& owner) :
            Asset<LayoutOffset, ILayoutOffset, UI>(ui, ui.getLayoutOffset(this, "Absolute", owner)),
            cOwner(owner) {
  }

  ILayoutOffset* LayoutOffset::createLiteralAsset(UI& ui) {
    return ui.createLiteralLayoutOffset(this, cOwner);
  }
  
  ILayoutOffset* LayoutOffset::getAsset(UI& ui, JSONObject object) {
    return ui.getLayoutOffset(this, object, cOwner);
  }
  
  ILayoutOffset* LayoutOffset::getAsset(UI& ui, const std::string& id) {
    return ui.getLayoutOffset(this, id, cOwner);
  }
  
  bool LayoutOffset::isDefaultConfiguration() const {
    return true;
  }
}
