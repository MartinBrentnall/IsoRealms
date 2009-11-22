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
#include "SpindizzyCraftPyramidModelFactory.h"

ISimpleModel* SpindizzyCraftPyramidModelFactory::createModel() {
  return new SpindizzyCraftPyramidModel();
}

void SpindizzyCraftPyramidModelFactory::setEditingInfo(IComponentContainer*) {
  // We don't need to know this.  Nothing to do.
}

std::string SpindizzyCraftPyramidModelFactory::getName() {
  return "Spindizzy Craft Pyramid Model";
}

std::vector<PlugSocket*> SpindizzyCraftPyramidModelFactory::getPlugSockets() {
  std::vector<PlugSocket*> mEmptyVector;
  return mEmptyVector;
}

void SpindizzyCraftPyramidModelFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  // TODO: Throw something
}

IPlugin* SpindizzyCraftPyramidModelFactory::getPlugin(PlugSocket* socket) {
  // TODO: Throw something
  return NULL;
}

void SpindizzyCraftPyramidModelFactory::initPlugin(Zone* zone) {
  // Nothing to do.
}

std::vector<ICommandInfo*> SpindizzyCraftPyramidModelFactory::getCommandInfo() {
  std::vector<ICommandInfo*> mEmptyVector;
  return mEmptyVector;
}

void SpindizzyCraftPyramidModelFactory::save(DOMNodeWriter* node) {
  // Nothing to do.
}

void SpindizzyCraftPyramidModelFactory::load(DOMNodeWrapper* node) {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new SpindizzyCraftPyramidModelFactory();
}

extern "C" void destroy(IPlugin* pyramidModel) {
  delete pyramidModel;
}
