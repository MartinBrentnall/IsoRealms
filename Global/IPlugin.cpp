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
