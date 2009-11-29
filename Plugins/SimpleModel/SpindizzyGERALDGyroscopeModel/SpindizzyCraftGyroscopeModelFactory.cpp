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
#include "SpindizzyCraftGyroscopeModelFactory.h"

ISimpleModel* SpindizzyCraftGyroscopeModelFactory::createModel() {
  return new SpindizzyCraftGyroscopeModel();
}

void SpindizzyCraftGyroscopeModelFactory::setEditingInfo(IComponentContainer*) {
  // We don't need to know this.  Nothing to do.
}

std::string SpindizzyCraftGyroscopeModelFactory::getName() {
  return "Spindizzy Craft Gyroscope Model";
}

std::vector<PlugSocket*> SpindizzyCraftGyroscopeModelFactory::getPlugSockets() {
  std::vector<PlugSocket*> mEmptyVector;
  return mEmptyVector;
}

void SpindizzyCraftGyroscopeModelFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  // TODO: Throw something
}

IPlugin* SpindizzyCraftGyroscopeModelFactory::getPlugin(PlugSocket* socket) {
  // TODO: Throw something
  return NULL;
}

void SpindizzyCraftGyroscopeModelFactory::notifyZoneAction(Zone* zone) {
  // Nothing to do.
}

void SpindizzyCraftGyroscopeModelFactory::initPlugin(Zone* zone) {
  // Nothing to do.
}

std::vector<ICommandInfo*> SpindizzyCraftGyroscopeModelFactory::getCommandInfo() {
  std::vector<ICommandInfo*> mEmptyVector;
  return mEmptyVector;
}

void SpindizzyCraftGyroscopeModelFactory::save(DOMNodeWriter* node) {
  // Nothing to do.
}

void SpindizzyCraftGyroscopeModelFactory::load(DOMNodeWrapper* node) {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new SpindizzyCraftGyroscopeModelFactory();
}

extern "C" void destroy(IPlugin* gyroscopeModel) {
  delete gyroscopeModel;
}
