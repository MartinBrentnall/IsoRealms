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
#include "SpindizzyJewelModelFactory.h"

ISimpleModel* SpindizzyJewelModelFactory::createModel() {
  return new SpindizzyJewelModel();
}

void SpindizzyJewelModelFactory::destroyModel(ISimpleModel* jewelModel) {
  delete jewelModel;
}

std::string SpindizzyJewelModelFactory::getName() {
  return "Spindizzy Jewel Model";
}

std::vector<PlugSocket*> SpindizzyJewelModelFactory::getPlugSockets() {
  std::vector<PlugSocket*> mEmptyVector;
  return mEmptyVector;
}

void SpindizzyJewelModelFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  // TODO: Throw something
}

IPlugin* SpindizzyJewelModelFactory::getPlugin(PlugSocket* socket) {
  // TODO: Throw something
  return NULL;
}

void SpindizzyJewelModelFactory::notifyZoneAction(Zone* zone) {
  // Nothing to do.
}

void SpindizzyJewelModelFactory::initPlugin(Zone* zone) {
  // Nothing to do.
}

void SpindizzyJewelModelFactory::setEditingInfo(IComponentContainer* componentContainer) {
  // We don't need to know this.  Nothing to do.
}

std::vector<ICommandInfo*> SpindizzyJewelModelFactory::getCommandInfo() {
  std::vector<ICommandInfo*> mEmptyVector;
  return mEmptyVector;
}

void SpindizzyJewelModelFactory::save(DOMNodeWriter* node) {
  // Nothing to do
}

void SpindizzyJewelModelFactory::load(DOMNodeWrapper* node) {
  // Nothing to do
}

extern "C" IPlugin* create() {
  return new SpindizzyJewelModelFactory();
}

extern "C" void destroy(IPlugin* jewelModelFactory) {
  delete jewelModelFactory;
}
