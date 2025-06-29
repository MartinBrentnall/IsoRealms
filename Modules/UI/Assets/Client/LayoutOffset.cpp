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
#include "LayoutOffset.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms::UI {
  LayoutOffset::LayoutOffset(IUI& ui, LayoutComponentEdge& owner) :
            Asset<LayoutOffset, ILayoutOffset, IUI>(ui, ui.getLayoutOffset(this, "Absolute", owner)),
            cOwner(owner) {
  }

  ILayoutOffset* LayoutOffset::createLiteralAsset(IUI& ui) {
    return ui.createLiteralLayoutOffset(this, cOwner);
  }
  
  ILayoutOffset* LayoutOffset::getAsset(IUI& ui, JSONObject object) {
    return ui.getLayoutOffset(this, object, cOwner);
  }
  
  ILayoutOffset* LayoutOffset::getAsset(IUI& ui, const std::string& id) {
    return ui.getLayoutOffset(this, id, cOwner);
  }
  
  std::vector<std::string> LayoutOffset::getAvailableProviders() const {
    return cManager.getAllLayoutOffsets();
  }  

  bool LayoutOffset::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderLayoutOffsetIcon(id);
  }

  bool LayoutOffset::hasConfiguration() const {
    return cManager.isLayoutOffsetConfigurable(getID());
  }

  bool LayoutOffset::isDefaultConfiguration() const {
    return true;
  }
}
