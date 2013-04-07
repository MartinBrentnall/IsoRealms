#include "DefaultZoneHandler.h"

void DefaultZoneHandler::render(std::vector<IZone*>& zones, IProject* project) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    project->renderPreZone(zones[i]);
    zones[i]->renderStatic();
  }
  for (unsigned int i = 0; i < zones.size(); i++) {
    project->renderPreZone(zones[i]);
    zones[i]->renderRuntime();
  }
}

void DefaultZoneHandler::updateEditing(std::vector<IZone*>& zones, unsigned int milliseconds) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->updateEditing(milliseconds);
  }
}

void DefaultZoneHandler::updateRuntime(std::vector<IZone*>& zones, unsigned int milliseconds) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->updateRuntime(milliseconds);
  }
}

void DefaultZoneHandler::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  // Nothing to do
}

