#include "DefaultZoneRenderer.h"

void DefaultZoneRenderer::render(std::vector<IZone*>& zones, IPluginRegistry& pluginRegistry) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->renderStatic();
  }
  for (unsigned int i = 0; i < zones.size(); i++) {
    pluginRegistry.renderPreZone(zones[i]);
    zones[i]->renderDynamic();
  }
}
