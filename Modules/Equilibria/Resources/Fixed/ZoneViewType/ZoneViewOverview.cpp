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
#include "ZoneViewOverview.h"

#include "Modules/Equilibria/Resources/Fixed/ZoneViewType/ZoneViewTypeOverview.h"
#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/Object/Zone/Zone.h"
#include "Modules/Equilibria/WorldView/WorldView.h"

namespace IsoRealms::Equilibria {
  ZoneViewOverview::ZoneViewOverview(WorldView& worldView, ZoneViewTypeOverview& type, Zone* zone) :
            cDefType(type),
            cLuaBinding(worldView.getEquilibria().getProject().getLuaState(), this) {
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
