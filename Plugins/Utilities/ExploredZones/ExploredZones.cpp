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
#include "ExploredZones.h"

ExploredZones::ExploredZones(IRuntimeContext* runtimeContext) {
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  assignDummyPlugin(&cObjectives, "Objectives");
  assignDummyPlugin(&cToGoStringProcessor, "StringProcessor");
  assignDummyPlugin(&cFlaggedZones, "FlaggedZones");
  cZoneCount = 0;
  cExploredZoneRenderer = new ExploredZoneRenderer(this);
  cMapOverviewRenderer = new MapOverviewRenderer(this);
  cZoneExploredScript = Script::getDummy();
  cAllZonesExploredScript = Script::getDummy();
  cRuntimeContext = runtimeContext;
}

void ExploredZones::initPlugin(IZone* zone, unsigned int pass) {
  if (pass == 0) {
    cZoneCount++;
    updateToGoString();
  }
}

void ExploredZones::zoneContextChanged(IZone* zone) {
  cZone = zone;
  if (zone != NULL && cExploredZones.find(zone) == cExploredZones.end()) {
    BlockArea* mBlockArea = zone->getZoneArea();
    float mXDifference = mBlockArea->getEast() - mBlockArea->getWest();
    float mYDifference = mBlockArea->getNorth() - mBlockArea->getSouth();
    float mZDifference = mBlockArea->getTop() - mBlockArea->getBottom();
    float mX = mBlockArea->getWest() + (mXDifference / 2.0f);
    float mY = mBlockArea->getSouth() + (mYDifference / 2.0f);
    float mZ = (mBlockArea->getBottom() + (mZDifference / 2.0f) - 1.0f) * IsoRealmsConstants::BLOCK_HEIGHT;
//    float mScale = std::min(mXDifference, std::min(mYDifference, mZDifference)) + 1;
    
    Vertex* mLocation = new Vertex(mX, mY, mZ);
    cExploredZones[zone] = cRuntimeContext->getModel(cModelPath, mLocation); // TODO: Scale
    cZoneExploredScript->execute();
    updateToGoString();
    cObjectives->check();
    if (isMet()) {
      cAllZonesExploredScript->execute();
    }
  }
}

void ExploredZones::updateToGoString() {
  std::stringstream mStringStream;
  mStringStream << cZoneCount - cExploredZones.size();
  cToGoString = mStringStream.str();
}

IZoneRenderer* ExploredZones::getZoneRenderer(const std::string& name) {
  if (name == "explored")    {return cExploredZoneRenderer;}
  if (name == "mapOverview") {return cMapOverviewRenderer;}
  return NULL;
}

void ExploredZones::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "Objectives") {
    IObjectives* mPreviousObjectives = cObjectives;
    if (assignPlugin(plugin, &cObjectives, *socket)) {
      mPreviousObjectives->unregisterObjective(this);
      cObjectives->registerObjective(this);
    }
  } else if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  } else if (socket->getType() == "FlaggedZones") {
    assignPlugin(plugin, &cFlaggedZones, *socket);
  } else if (socket->getType() == "StringProcessor") {
    assignPlugin(plugin, &cToGoStringProcessor, *socket);
    cToGoStringProcessor->registerString(&cToGoString);
  } else {
    // TODO: Throw
  }
}

IPlugin* ExploredZones::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "Objectives")      {return cObjectives;}
  if (socket->getType() == "ZoneContext")     {return cZoneContext;}
  if (socket->getType() == "FlaggedZones")    {return cFlaggedZones;}
  if (socket->getType() == "StringProcessor") {return cToGoStringProcessor;}
  return NULL;
}

ExploredZones::ExploredZoneRenderer::ExploredZoneRenderer(ExploredZones* parent) {
  cParent = parent;
}

void ExploredZones::ExploredZoneRenderer::render(std::vector<IZone*>& zones, IPluginRegistry& pluginRegistry) {
  for (std::map<IZone*, I3DModel*>::iterator i = cParent->cExploredZones.begin(); i != cParent->cExploredZones.end(); ++i) {
    i->first->renderStatic();
  }
  for (std::map<IZone*, I3DModel*>::iterator i = cParent->cExploredZones.begin(); i != cParent->cExploredZones.end(); ++i) {
    pluginRegistry.renderPreZone(i->first);
    i->first->renderDynamic();
  }
}

void ExploredZones::ExploredZoneRenderer::update(std::vector<IZone*>& zones, unsigned int milliseconds) {
  for (std::map<IZone*, I3DModel*>::iterator i = cParent->cExploredZones.begin(); i != cParent->cExploredZones.end(); ++i) {
    i->first->update(milliseconds);
  }
}

void ExploredZones::ExploredZoneRenderer::updateRuntime(std::vector<IZone*>& zones, unsigned int milliseconds) {
  for (std::map<IZone*, I3DModel*>::iterator i = cParent->cExploredZones.begin(); i != cParent->cExploredZones.end(); ++i) {
    i->first->updateRuntime(milliseconds);
  }
}

ExploredZones::MapOverviewRenderer::MapOverviewRenderer(ExploredZones* parent) {
  cParent = parent;
}

void ExploredZones::MapOverviewRenderer::render(std::vector<IZone*>& zones, IPluginRegistry& pluginRegistry) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    pluginRegistry.renderPreZone(zones[i]);
    glBindTexture(GL_TEXTURE_2D, 0);
    glLineWidth(2.0);
    std::map<IZone*, I3DModel*>::iterator j = cParent->cExploredZones.find(zones[i]);
    if (j != cParent->cExploredZones.end()) {
      if (cParent->cFlaggedZones->isZoneFlagged(zones[i])) {
        j->second->render();
      }
      if (zones[i] == cParent->cZone) {
        glColor3f(1.0f, 0.3f, 0.3f);
      } else {
        glColor3f(0.8f, 1.0f, 1.0f);
      }
    } else {
      glColor3f(0.0f, 0.2f, 0.3f);
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

void ExploredZones::MapOverviewRenderer::update(std::vector<IZone*>& zones, unsigned int milliseconds) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    std::map<IZone*, I3DModel*>::iterator j = cParent->cExploredZones.find(zones[i]);
    if (j != cParent->cExploredZones.end() && cParent->cFlaggedZones->isZoneFlagged(zones[i])) {
      j->second->update(milliseconds);
    }
  }
}

void ExploredZones::MapOverviewRenderer::updateRuntime(std::vector<IZone*>& zones, unsigned int milliseconds) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    std::map<IZone*, I3DModel*>::iterator j = cParent->cExploredZones.find(zones[i]);
    if (j != cParent->cExploredZones.end() && cParent->cFlaggedZones->isZoneFlagged(zones[i])) {
      j->second->update(milliseconds);
    }
  }
}

void ExploredZones::save(DOMNodeWriter* node) {
  cZoneExploredScript->save(node, "ZoneExploredScript");
  cAllZonesExploredScript->save(node, "AllZonesExploredScript");
}

void ExploredZones::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ZoneExploredScript") {
      cZoneExploredScript = cRuntimeContext->getScript(mNode);
    } else if (mValueAsString == "AllZonesExploredScript") {
      cAllZonesExploredScript = cRuntimeContext->getScript(mNode);
    } else if (mValueAsString == "ZoneFlag") {
      cModelPath = mNode->getAttribute("name");
    } else {
      // TODO: Throw something!
    }
  }
}

bool ExploredZones::isMet() {
  return cExploredZones.size() == cZoneCount;
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new ExploredZones(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
