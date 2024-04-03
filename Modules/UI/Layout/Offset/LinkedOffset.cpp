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
#include "LinkedOffset.h"

#include "Modules/UI/Layout/Layout.h"

namespace IsoRealms::UI {
  const std::string LinkedOffset::ATTRIBUTE_LINKED = "linked";
  const std::string LinkedOffset::ATTRIBUTE_RATIO  = "ratio";
  const std::string LinkedOffset::ATTRIBUTE_VALUE  = "value";

  const std::string LinkedOffset::VALUE_HEIGHT = "Height";
  const std::string LinkedOffset::VALUE_WIDTH  = "Width";
  
  LinkedOffset::LinkedOffset(IProject* project, DOMNode& node, Layout* layout, LayoutComponent* defaultLink, float defaultRatio) {
    cDefHorizontal = node.getAttribute(ATTRIBUTE_VALUE) == VALUE_WIDTH;
    cDefRatio      = node.getFloatAttribute(ATTRIBUTE_RATIO, defaultRatio);
    project->init([this, node, layout, defaultLink](IAssets* assets) {
      std::string mLinkedName = node.getAttribute(ATTRIBUTE_LINKED);
      cDefLinked = mLinkedName == "" ? defaultLink : layout->getComponent(mLinkedName);
    });
  }
  
  float LinkedOffset::getOffset(float aspectRatio) const {
    float mComponentStart = cDefHorizontal ? cDefLinked->getLeft(aspectRatio)  : cDefLinked->getBottom();
    float mComponentEnd   = cDefHorizontal ? cDefLinked->getRight(aspectRatio) : cDefLinked->getTop();
    return (mComponentEnd - mComponentStart) * cDefRatio;
  }
  
  void LinkedOffset::save(DOMNodeWriter* node, Layout* layout) const {
    node->addAttribute("type", std::string("Linked"));
    node->addAttribute(ATTRIBUTE_VALUE, cDefHorizontal ? VALUE_WIDTH : VALUE_HEIGHT);
    node->addAttribute(ATTRIBUTE_LINKED, layout->getName(cDefLinked));
  }  
}
