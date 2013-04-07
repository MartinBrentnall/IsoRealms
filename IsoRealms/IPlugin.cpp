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
#include "IPlugin.h"

void IPlugin::notifyZoneAction(IZone*) {
  // Nothing to do.
}

void IPlugin::initPlugin(IZone*, unsigned int) {
  // Nothing to do.
}

void IPlugin::renderPreZone(IZone*) {
  // Nothing to do.
}

void IPlugin::zoneContextChanged(IMap*, IZone*) {
  // Nothing to do.
}

void IPlugin::setEditingContext(IEditingContext*, IResourceManager*) {
  // Nothing to do.
}

void IPlugin::save(DOMNodeWriter*, IResourceLocator*) {
  // Nothing to do.
}

void IPlugin::saveData(DOMNodeWriter*, IMap*, IZone*) {
  // Nothing to do.
}

void IPlugin::loadData(DOMNodeWrapper*, IPluginRegistry*, IZone*) {
  // Nothing to do.
}

void IPlugin::initZone(IZone* zone) {
  // Nothing to do
}

void IPlugin::setPluginRegistry(IPluginRegistry* pluginRegistry) {
  cPluginRegistry = pluginRegistry;
}

IZoneHandler* IPlugin::getZoneRenderer(const std::string& name) {
  return NULL;
}


