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
  cModelInstance = new SpindizzyCraftBallModel();
}

ISimpleModel* SpindizzyCraftBallModelFactory::createModel() {
  return cModelInstance;  
}

void SpindizzyCraftBallModelFactory::setEditingInfo(IComponentContainer*) {
  // We don't need to know this.  Nothing to do.
}

std::string SpindizzyCraftBallModelFactory::getName() {
  return "Spindizzy Craft Ball Model";
}

std::vector<PlugSocket*> SpindizzyCraftBallModelFactory::getPlugSockets() {
  std::vector<PlugSocket*> mEmptyVector;
  return mEmptyVector;
}

void SpindizzyCraftBallModelFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  // TODO: Throw something
}

IPlugin* SpindizzyCraftBallModelFactory::getPlugin(PlugSocket* socket) {
  // TODO: Throw something
  return NULL;
}

void SpindizzyCraftBallModelFactory::notifyZoneAction(Zone* zone) {
  // Nothing to do.
}

void SpindizzyCraftBallModelFactory::initPlugin(Zone* zone) {
  // Nothing to do.
}

std::vector<ICommandInfo*> SpindizzyCraftBallModelFactory::getCommandInfo() {
  std::vector<ICommandInfo*> mEmptyVector;
  return mEmptyVector;
}

void SpindizzyCraftBallModelFactory::save(DOMNodeWriter* node) {
  // Nothing to do.
}

void SpindizzyCraftBallModelFactory::load(DOMNodeWrapper* node) {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new SpindizzyCraftBallModelFactory();
}

extern "C" void destroy(IPlugin* ballModel) {
  delete ballModel;
}
