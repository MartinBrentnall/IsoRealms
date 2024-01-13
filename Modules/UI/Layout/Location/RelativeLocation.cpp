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
#include "RelativeLocation.h"

#include "Modules/UI/Layout/Layout.h"

namespace IsoRealms::UI {
  const std::string RelativeLocation::ATTRIBUTE_RELATIVE = "relative";
  const std::string RelativeLocation::ATTRIBUTE_VALUE    = "value";
  
  RelativeLocation::RelativeLocation(IProject* project, DOMNode& node, Layout* layout, bool horizontal, float defaultValue) :
            cDefRelative(nullptr),
            cDefHorizontal(horizontal),
            cDefValue(node.getFloatAttribute(ATTRIBUTE_VALUE, defaultValue)) {
    project->init([this, node, layout](IAssets* assets) {
      cDefRelative = layout->getComponent(node.getAttribute(ATTRIBUTE_RELATIVE));
    });
  }
  
  float RelativeLocation::getLocation(float aspectRatio) const {
    float mComponentStart = cDefHorizontal ? cDefRelative->getLeft(aspectRatio)  : cDefRelative->getBottom();
    float mComponentEnd   = cDefHorizontal ? cDefRelative->getRight(aspectRatio) : cDefRelative->getTop();
    return mComponentStart + ((mComponentEnd - mComponentStart) / 2.0f) * (cDefValue + 1.0f);
  }
  
  void RelativeLocation::save(DOMNodeWriter* node, Layout* layout) const {
    // TODO: Only save if not default value
    node->addAttribute(ATTRIBUTE_VALUE, cDefValue);
    node->addAttribute(ATTRIBUTE_RELATIVE, layout->getName(cDefRelative));
  }  
}
