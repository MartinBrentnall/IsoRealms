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

std::vector<IDynamicElement*> IPlugin::cNoDynamicElements;
std::vector<IInteractiveElement*> IPlugin::cNoInteractiveElements;
std::vector<ICommandInfo*> IPlugin::cNoCommands;

void IPlugin::notifyZoneAction(IZone*) {
  // Nothing to do.
}

void IPlugin::initPlugin(IZone*) {
  // Nothing to do.
}

std::vector<IDynamicElement*> IPlugin::getPreLoopCommands() {
  return cNoDynamicElements;
}

std::vector<IDynamicElement*> IPlugin::getPostLoopCommands() {
  return cNoDynamicElements;
}

std::vector<IInteractiveElement*> IPlugin::getInteractiveElements() {
  return cNoInteractiveElements;
}

void IPlugin::renderPreZone(IZone*) {
  // Nothing to do.
}

void IPlugin::zoneContextChanged(IMap*, IZone*) {
  // Nothing to do.
}

void IPlugin::setEditingContext(BlockLocation*, IComponentContainer*) {
  // Nothing to do.
}

std::vector<ICommandInfo*> IPlugin::getCommandInfo() {
  return cNoCommands;
}

void IPlugin::save(DOMNodeWriter*) {
  // Nothing to do.
}

void IPlugin::saveData(DOMNodeWriter*, IMap*, IZone*) {
  // Nothing to do.
}

void IPlugin::load(DOMNodeWrapper*) {
  // Nothing to do.
}

void IPlugin::loadData(DOMNodeWrapper*, IPluginRegistry*, IZone*) {
  // Nothing to do.
}
