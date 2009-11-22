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
#include "DummyPlugin.h"

std::string DummyPlugin::getName() {
  return "";
}

std::vector<PlugSocket*> DummyPlugin::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  return mSockets;
}

void DummyPlugin::setPlugin(PlugSocket*, IPlugin*) {
  // Nothing to do // TODO: Throw exception
}

IPlugin* DummyPlugin::getPlugin(PlugSocket*) {
  // TODO Throw exception
  return NULL;
}

void DummyPlugin::initPlugin(Zone*) {
  // Nothing to do.
}

void DummyPlugin::setEditingInfo(IComponentContainer* componentContainer) {
  // We don't need to know.  Nothing to do.
}

std::vector<ICommandInfo*> DummyPlugin::getCommandInfo() {
  std::vector<ICommandInfo*> mEmptyVector;
  return mEmptyVector;
}

void DummyPlugin::save(DOMNodeWriter* node) {
  // Nothing to do.
}

void DummyPlugin::load(DOMNodeWrapper* node) {
  // Nothing to do.
}




