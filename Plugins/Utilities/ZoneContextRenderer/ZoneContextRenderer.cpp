/*
 * Copyright 2009 Martin Brentnall
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
#include "ZoneContextRenderer.h"

ZoneContextRenderer::ZoneContextRenderer() {
  assignDummyPlugin(&cZoneContext, "ZoneContext");
}

IZoneRenderer* ZoneContextRenderer::getZoneRenderer(const std::string& name) {
  if (name == "zone") {return this;}
  return NULL;
}

void ZoneContextRenderer::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* ZoneContextRenderer::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "ZoneContext") {return cZoneContext;}
  // TODO: Throw
  return NULL;
}

void ZoneContextRenderer::render(std::vector<IZone*>& zones, IProject* project) {
  if (cZone != NULL) {
    cZone->renderStatic();
    project->renderPreZone(cZone);
    cZone->renderDynamic();
  }
}

void ZoneContextRenderer::update(std::vector<IZone*>& zones, unsigned int milliseconds) {
  if (cZone != NULL) {
    cZone->update(milliseconds);
  }
}

void ZoneContextRenderer::updateRuntime(std::vector<IZone*>& zones, unsigned int milliseconds) {
  // TODO: We update all zones to do resets for out of context zones.  There is surely a more performance friendly way to do this.
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->updateRuntime(milliseconds);
  }
}

void ZoneContextRenderer::zoneContextChanged(IZone* zone) {
  if (zone != NULL) {
    cZone = zone;
  }
}

extern "C" IPlugin* create() {
  return new ZoneContextRenderer();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

