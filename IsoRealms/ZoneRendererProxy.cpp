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
#include "ZoneRendererProxy.h"

ZoneRendererProxy::ZoneRendererProxy(IZoneRenderer* zoneRenderer, bool active, ICommandRegistry* commandRegistry, const std::string& rendererName, const std::string& type, const std::string& instance) {
  cZoneRenderer = zoneRenderer;
  cActive = active;
  cName = rendererName;
  commandRegistry->registerCommand(new RendererActivationCommand(this, true));
  commandRegistry->registerCommand(new RendererActivationCommand(this, false));
  cType = type;
  cInstance = instance;
}

void ZoneRendererProxy::save(DOMNodeWriter* node) {
  DOMNodeWriter* mZoneRendererBranch = node->addBranch("ZoneRenderer");
  mZoneRendererBranch->addAttribute("type", cType);
  mZoneRendererBranch->addAttribute("instance", cInstance);
  mZoneRendererBranch->addAttribute("renderer", cName);
  mZoneRendererBranch->addAttribute("active", cActive ? "true" : "false");
}

void ZoneRendererProxy::render(std::vector<IZone*>& zones, IPluginRegistry& pluginRegistry) {
  if (cActive) {
    cZoneRenderer->render(zones, pluginRegistry);
  }
}

void ZoneRendererProxy::update(std::vector<IZone*>& zones, unsigned int milliseconds) {
  if (cActive) { // TODO: Only when active?
    cZoneRenderer->update(zones, milliseconds);
  }
}

void ZoneRendererProxy::updateRuntime(std::vector<IZone*>& zones, unsigned int milliseconds) {
  if (cActive) { // TODO: Only when active?
    cZoneRenderer->updateRuntime(zones, milliseconds);
  }
}

ZoneRendererProxy::RendererActivationCommand::RendererActivationCommand(ZoneRendererProxy* parent, bool activate) {
  cParent = parent;
  cActivate = activate;
}

void ZoneRendererProxy::RendererActivationCommand::execute() {
  cParent->cActive = cActivate;
}

std::string ZoneRendererProxy::RendererActivationCommand::getCommandName() {
  return cParent->cName + (cActivate ? "_Activate" : "_Deactivate");
}
