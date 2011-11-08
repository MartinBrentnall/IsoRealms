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
#include "SpindizzyCraftBallModelFactory.h"

SpindizzyCraftBallModelFactory::SpindizzyCraftBallModelFactory() {
  assignDummyPlugin(&cCamera, "Camera");
}

ISimpleModel* SpindizzyCraftBallModelFactory::createModel(Vertex* location, float scale) {
  return new SpindizzyCraftBallModel(location, cCamera);
}

void SpindizzyCraftBallModelFactory::destroyModel(ISimpleModel* ballModel) {
  delete ballModel;
}

void SpindizzyCraftBallModelFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "Camera") {
    assignPlugin(plugin, &cCamera, *socket);
  } else {
    // TODO: Throw something
  }
}

IPlugin* SpindizzyCraftBallModelFactory::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "Camera") {return cCamera;}
  // TODO: Throw something
  return NULL;
}

extern "C" IPlugin* create() {
  return new SpindizzyCraftBallModelFactory();
}

extern "C" void destroy(IPlugin* ballModel) {
  delete ballModel;
}
