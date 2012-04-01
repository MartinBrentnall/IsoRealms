#include "DefaultZoneRenderer.h"

void DefaultZoneRenderer::render(std::vector<IZone*>& zones, IProject* project) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->renderStatic();
  }
  for (unsigned int i = 0; i < zones.size(); i++) {
    project->renderPreZone(zones[i]);
    zones[i]->renderDynamic();
  }
}

void DefaultZoneRenderer::update(std::vector<IZone*>& zones, unsigned int milliseconds) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->update(milliseconds);
  }
}

void DefaultZoneRenderer::updateRuntime(std::vector<IZone*>& zones, unsigned int milliseconds) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->updateRuntime(milliseconds);
  }
}
