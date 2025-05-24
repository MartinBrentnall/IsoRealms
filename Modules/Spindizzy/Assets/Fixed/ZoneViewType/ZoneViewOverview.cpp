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
#include "ZoneViewOverview.h"

#include "Modules/Spindizzy/World/Object/Zone/Zone.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneViewType/ZoneViewTypeOverview.h"

namespace IsoRealms::Spindizzy {
  ZoneViewOverview::ZoneViewOverview(IProject& project, WorldView& worldView, ZoneViewTypeOverview& type, Zone* zone) :
            cDefType(type),
            cLuaBinding(project, this) {
    zone->addProperty(this);
  }

  void ZoneViewOverview::setColour(IColour* colour) {
    cRuntimeColour = colour;
  }

  const IColour* ZoneViewOverview::getColour() {
    return cRuntimeColour;
  }
  
  void ZoneViewOverview::render(Zone* zone, const IScreen* screen) const {
    cRuntimeColour->set();
    float mStartX = zone->getStartX() - 0.5f;
    float mStartY = zone->getStartY() - 0.5f;
    float mStartZ = zone->getStartZ() * 0.5f;
    float mEndX   = zone->getEndX()   + 0.5f;
    float mEndY   = zone->getEndY()   + 0.5f;

    glBegin(GL_QUADS);
    glVertex3f(mEndX,   mEndY,   mStartZ);
    glVertex3f(mStartX, mEndY,   mStartZ);
    glVertex3f(mStartX, mStartY, mStartZ);
    glVertex3f(mEndX,   mStartY, mStartZ);
    glEnd();
  }
  
  void ZoneViewOverview::reset() {
    cRuntimeColour = cDefType.getInitialColour();
  }

  void ZoneViewOverview::bindProperty() {
    cDefType.bind1(this);
  }

  void ZoneViewOverview::bindProperty2() {
    cDefType.bind2(this);
  }

  void ZoneViewOverview::unbindProperty() {
    cDefType.bind1(nullptr);
  }

  void ZoneViewOverview::unbindProperty2() {
    cDefType.bind2(nullptr);
  }
}
