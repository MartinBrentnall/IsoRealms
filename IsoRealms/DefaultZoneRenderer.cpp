#include "DefaultZoneRenderer.h"

void DefaultZoneRenderer::render(std::vector<IZone*>& zones) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->renderStatic();
  }
  for (unsigned int i = 0; i < zones.size(); i++) {
    zones[i]->renderDynamic();
  }
}
