/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "DefaultZoneContext.h"

DefaultZoneContext::DefaultZoneContext() {
  assignDummyPlugin(&cLocationAwareness, "LocationAwareness");
}

void DefaultZoneContext::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "LocationAwareness") {
    if (assignPlugin(plugin, &cLocationAwareness, *socket)) {
      cLocationAwareness->setLocation(&cLocation);
    }
  } else {
    // TODO: Throw something
  }
}

IPlugin* DefaultZoneContext::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "LocationAwareness") {return cLocationAwareness;}
  // TODO: Throw
  return NULL;
}
  
void DefaultZoneContext::setZoneContext(IZone* context) {
  if (context != NULL) {
    BlockArea* mZoneArea = context->getZoneArea();
    cLocation.x = mZoneArea->getWest()   + (mZoneArea->getEast()  - mZoneArea->getWest())   / 2.0f;
    cLocation.y = mZoneArea->getSouth()  + (mZoneArea->getNorth() - mZoneArea->getSouth())  / 2.0f;
    cLocation.z = mZoneArea->getBottom() + (mZoneArea->getTop()   - mZoneArea->getBottom()) / 2.0f;
  }
  cContext = context;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->zoneContextChanged(cContext);
  }
}

IZone* DefaultZoneContext::getZoneContext() {
  return cContext;
}

void DefaultZoneContext::addZoneContextListener(IZoneContextListener* listener) {
  cListeners.push_back(listener);
}

void DefaultZoneContext::removeZoneContextListener(IZoneContextListener* listener) {
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    if (cListeners[i] == listener) {
      cListeners.erase(cListeners.begin() + i);
    }
  }
}

extern "C" IPlugin* create() {
  return new DefaultZoneContext();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
