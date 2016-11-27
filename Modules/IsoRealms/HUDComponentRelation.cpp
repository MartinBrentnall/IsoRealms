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

HUDComponentRelation::HUDComponentRelation(HUDComponentProxy* relative, std::vector<std::string> description, const std::string& edge) {
  cRelative = relative;
  cRelationType = description[0] == "adjacent" ? ADJACENT
                : description[0] == "align"    ? ALIGNED
                :                                CENTER;
  cEdge = edge == "left"   ? LEFT
        : edge == "right"  ? RIGHT
        : edge == "bottom" ? BOTTOM
                           : TOP;
  cOffset = description.size() >= 3 ? atof(description[2].c_str()) : 0.0f;
}

float HUDComponentRelation::getLocation() {
  switch (cEdge) {
    case LEFT:   return cRelationType == ADJACENT ? cRelative->getEast()                                  + cOffset
                      : cRelationType == ALIGNED  ? cRelative->getWest()                                  + cOffset
                      :                            (cRelative->getEast() + cRelative->getWest())   / 2.0f + cOffset;
    case RIGHT:  return cRelationType == ADJACENT ? cRelative->getWest()                                  - cOffset
                      : cRelationType == ALIGNED  ? cRelative->getEast()                                  - cOffset
                      :                            (cRelative->getEast()  + cRelative->getWest())  / 2.0f - cOffset;
    case BOTTOM: return cRelationType == ADJACENT ? cRelative->getNorth()                                 + cOffset
                      : cRelationType == ALIGNED  ? cRelative->getSouth()                                 + cOffset
                      :                            (cRelative->getNorth() + cRelative->getSouth()) / 2.0f + cOffset;
    case TOP:    return cRelationType == ADJACENT ? cRelative->getSouth()                                 - cOffset
                      : cRelationType == ALIGNED  ? cRelative->getNorth()                                 - cOffset
                      :                            (cRelative->getNorth() + cRelative->getSouth()) / 2.0f + cOffset;
  }
  std::cout << "WARNING: Edge type not known" << std::endl;
  return 0.0f;
}

void HUDComponentRelation::save(DOMNodeWriter* node, const std::string& name, IComponentSources* sources) {
  std::string mSource = cRelative->getSource(sources);
  node->addAttribute(name, (cRelationType == ADJACENT ? "adjacent " : "align ") + mSource); 
}
