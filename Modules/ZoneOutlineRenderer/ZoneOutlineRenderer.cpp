#include "ZoneOutlineRenderer.h"

void ZoneOutlineRenderer::render(std::vector<IZone*>& zones, IProject* project) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    project->renderPreZone(zones[i]);
    glBindTexture(GL_TEXTURE_2D, 0);
    glLineWidth(2.0);
    cColour->set();
    BlockArea* mZoneArea = zones[i]->getZoneArea();
    float x = mZoneArea->getWest()          * IsoRealmsConstants::BLOCK_SIZE   - IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float y = mZoneArea->getSouth()         * IsoRealmsConstants::BLOCK_SIZE   - IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float z = mZoneArea->getTop()           * IsoRealmsConstants::BLOCK_HEIGHT - IsoRealmsConstants::BLOCK_HEIGHT * 0.25f;
    float xs = mZoneArea->getEast()         * IsoRealmsConstants::BLOCK_SIZE   + IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float ys = mZoneArea->getNorth()        * IsoRealmsConstants::BLOCK_SIZE   + IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float zs = (mZoneArea->getBottom() - 1) * IsoRealmsConstants::BLOCK_HEIGHT + IsoRealmsConstants::BLOCK_HEIGHT * 0.25f;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_LINES);
    glVertex3f(xs, ys, z); glVertex3f(x,  ys, z);
    glVertex3f(x,  ys, z); glVertex3f(x,  y,  z);
    glVertex3f(x,  y,  z); glVertex3f(xs, y,  z);
    glVertex3f(xs, y,  z); glVertex3f(xs, ys, z);

    glVertex3f(xs, ys, zs);   glVertex3f(x,  ys, zs);
    glVertex3f(x,  ys, zs);   glVertex3f(x,  y,  zs);
    glVertex3f(x,  y,  zs);   glVertex3f(xs, y,  zs);
    glVertex3f(xs, y,  zs);   glVertex3f(xs, ys, zs);

    glVertex3f(x,  ys, z); glVertex3f(x,  ys, zs);
    glVertex3f(x,  y,  z); glVertex3f(x,  y,  zs);
    glVertex3f(xs, y,  z); glVertex3f(xs, y,  zs);
    glVertex3f(xs, ys, z); glVertex3f(xs, ys, zs);
    glEnd();    
  }
}

void ZoneOutlineRenderer::update(std::vector<IZone*>& zones, unsigned int ticks) {
  // Nothing to do
}

void ZoneOutlineRenderer::updateRuntime(std::vector<IZone*>& zones, unsigned int ticks) {
  // Nothing to do
}

void ZoneOutlineRenderer::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  std::string mColour = node->getAttribute("colour");
  cColour = resources->getColour(mColour);
}

void ZoneOutlineRenderer::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("colour", resourceLocator->getPath(cColour));
}

