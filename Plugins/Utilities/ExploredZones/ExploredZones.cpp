#include "ExploredZones.h"

ExploredZones::ExploredZones() {
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cSockets.push_back(new PlugSocket("CommandRegistry"));
  cSockets.push_back(new PlugSocket("ZoneContext"));
  cZoneCount = 0;
  cExploredZoneRenderer = new ExploredZoneRenderer(this);
  cMapOverviewRenderer = new MapOverviewRenderer(this);
}

void ExploredZones::initPlugin(IZone* zone) {
  cZoneCount++;
}

void ExploredZones::zoneContextChanged(IZone* zone) {
  if (zone != NULL && cExploredZones.find(zone) == cExploredZones.end()) {
    cExploredZones.insert(zone);
    for (unsigned int i = 0; i < cZoneExploredCommands.size(); i++) {
      cZoneExploredCommands[i]->execute();
    }
    if (cExploredZones.size() == cZoneCount) {
      for (unsigned int i = 0; i < cAllZonesExploredCommands.size(); i++) {
        cAllZonesExploredCommands[i]->execute();
      }
    }
  }
}

IZoneRenderer* ExploredZones::getZoneRenderer(const std::string& name) {
  if (name == "explored")    {return cExploredZoneRenderer;}
  if (name == "mapOverview") {return cMapOverviewRenderer;}
  return NULL;
}

std::vector<PlugSocket*> ExploredZones::getPlugSockets() {
  return cSockets;
}

void ExploredZones::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    assignPlugin(plugin, &cCommandRegistry, *socket);
  } else if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* ExploredZones::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  if (socket->getType() == "ZoneContext")     {return cZoneContext;}
  return NULL;
}

ExploredZones::ExploredZoneRenderer::ExploredZoneRenderer(ExploredZones* parent) {
  cParent = parent;
}

void ExploredZones::ExploredZoneRenderer::render(std::vector<IZone*>& zones) {
  for (std::set<IZone*>::iterator i = cParent->cExploredZones.begin(); i != cParent->cExploredZones.end(); ++i) {
    (*i)->render();
  }
}

ExploredZones::MapOverviewRenderer::MapOverviewRenderer(ExploredZones* parent) {
  cParent = parent;
}

void ExploredZones::MapOverviewRenderer::render(std::vector<IZone*>& zones) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glLineWidth(2.0);
    if (cParent->cExploredZones.find(zones[i]) != cParent->cExploredZones.end()) {
      glColor3f(0.8, 1.0, 1.0);
    } else {
      glColor3f(0.0, 0.2, 0.3);
    }
    BlockArea* mZoneArea = zones[i]->getZoneArea();
    float x = mZoneArea->getWest()    * IsoRealmsConstants::BLOCK_SIZE   - IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float y = mZoneArea->getSouth()   * IsoRealmsConstants::BLOCK_SIZE   - IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float z = mZoneArea->getTop()     * IsoRealmsConstants::BLOCK_HEIGHT - IsoRealmsConstants::BLOCK_HEIGHT * 0.25f;
    float xs = mZoneArea->getEast()   * IsoRealmsConstants::BLOCK_SIZE   + IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
    float ys = mZoneArea->getNorth()  * IsoRealmsConstants::BLOCK_SIZE   + IsoRealmsConstants::BLOCK_RADIUS * 0.5f;
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

extern "C" IPlugin* create() {
  return new ExploredZones();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
