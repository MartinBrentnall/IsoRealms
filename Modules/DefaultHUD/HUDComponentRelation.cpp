/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "HUDComponentRelation.h"

HUDComponentRelation::HUDComponentRelation(HUDComponentProxy* relative, const std::string& relationType, const std::string& edge) {
  cRelative = relative;
  cRelationType = relationType == "adjacent" ? ADJACENT
                                             : ALIGNED;
  cEdge = edge == "left"   ? LEFT
        : edge == "right"  ? RIGHT
        : edge == "bottom" ? BOTTOM
                           : TOP;
}

float HUDComponentRelation::getLocation() {
  switch (cEdge) {
    case LEFT:   return cRelationType == ADJACENT ? cRelative->getRight()  : cRelative->getLeft();
    case RIGHT:  return cRelationType == ADJACENT ? cRelative->getLeft()   : cRelative->getRight();
    case TOP:    return cRelationType == ADJACENT ? cRelative->getBottom() : cRelative->getTop();
    case BOTTOM: return cRelationType == ADJACENT ? cRelative->getTop()    : cRelative->getBottom();
  }
  std::cout << "WARNING: Edge type not known" << std::endl;
  return 0.0f;
}

void HUDComponentRelation::save(DOMNodeWriter* node, const std::string& name, IComponentSources* sources) {
  std::string mSource = cRelative->getSource(sources);
  node->addAttribute(name, (cRelationType == ADJACENT ? "adjacent " : "align ") + mSource); 
}
