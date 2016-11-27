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
#include "ScreenRelative.h"

ScreenRelative ScreenRelative::LEFT_EDGE(  ScreenRelative::LEFT);
ScreenRelative ScreenRelative::RIGHT_EDGE( ScreenRelative::RIGHT);
ScreenRelative ScreenRelative::BOTTOM_EDGE(ScreenRelative::BOTTOM);
ScreenRelative ScreenRelative::TOP_EDGE(   ScreenRelative::TOP);

ScreenRelative::ScreenRelative(ScreenRelative::ScreenEdge edge) {
  cEdge = edge;
}

float ScreenRelative::getLocation() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return cEdge == LEFT   ? -1.0f / mAspectRatio
       : cEdge == RIGHT  ?  1.0f / mAspectRatio
       : cEdge == BOTTOM ? -1.0f
       :                    1.0f;
}

void ScreenRelative::save(DOMNodeWriter* node, const std::string& attribute, IComponentSources*) {
  node->addAttribute(attribute, getLocation());
}

