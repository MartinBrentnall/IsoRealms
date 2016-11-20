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
#include "ResourceModelSpindizzyJewel.h"

ResourceModelSpindizzyJewel::ResourceModelSpindizzyJewel(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cColourFrame = new Colour(1.0f, 1.0f, 0.0f);
  cColoursCycle.push_back(new Colour(0.0f, 0.0f, 0.0f));
  cColoursCycle.push_back(new Colour(1.0f, 0.0f, 0.0f));
  Vertex* mVertex = new Vertex(0.0f, 0.0f, 0.0f);
  cSampleModel = new ModelSpindizzyJewel(mVertex, 1.0f, &cColoursCycle, &cColourFrame, &cCycleSpeed);
}

IColour* ResourceModelSpindizzyJewel::getFrameColour() {
  return cColourFrame;
}

std::vector<IColour*> ResourceModelSpindizzyJewel::getCycleColours() {
  return cColoursCycle;
}

void ResourceModelSpindizzyJewel::setFrameColour(IColour* colour) {
  cColourFrame = colour;
}

void ResourceModelSpindizzyJewel::setCycleColour(unsigned int index, IColour* colour) {
  cColoursCycle[index] = colour;
}

I3DModel* ResourceModelSpindizzyJewel::createModel(Vertex* location, float scale) {
  ModelSpindizzyJewel* mJewelModel = new ModelSpindizzyJewel(location, scale, &cColoursCycle, &cColourFrame, &cCycleSpeed);
  cInstances.push_back(mJewelModel);
  return mJewelModel;
}

void ResourceModelSpindizzyJewel::destroyModel(I3DModel* model) {
  delete model;
}

void ResourceModelSpindizzyJewel::updateIcon(unsigned int milliseconds) {
  cSampleModel->update(milliseconds);
}

void ResourceModelSpindizzyJewel::renderIcon() {
  glTranslatef(0.0f, -0.5f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleModel->render();
}

void ResourceModelSpindizzyJewel::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  node->addAttribute("frameColour", resourceLocator->getPath(cColourFrame));
  node->addAttribute("cycleSpeed", cCycleSpeed);
  for (unsigned int i = 0; i < cColoursCycle.size(); i++) {
    DOMNodeWriter* mNode = node->addBranch("ColourCycle");
    mNode->addAttribute("colour", resourceLocator->getPath(cColoursCycle[i]));
  }
}

void ResourceModelSpindizzyJewel::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  std::string mFrameColour = node->getAttribute("frameColour");
  cCycleSpeed = node->getFloatAttribute("cycleSpeed");
  cColourFrame = resourceAccessor->getColour(mFrameColour);
  cColoursCycle.clear();
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ColourCycle") {
      std::string mColour = mNode->getAttribute("colour");
      IColour* mCycleColour = resourceAccessor->getColour(mColour);
      cColoursCycle.push_back(mCycleColour);
    }
  }
}

void ResourceModelSpindizzyJewel::resourceChanged(IColour* colour) {
  // No action necessary
}

void ResourceModelSpindizzyJewel::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
  bool mChanged = false;
  if (destroyee == cColourFrame) {
    cColourFrame = replacement;
    mChanged = true;
  }
  for (unsigned int i = cColoursCycle.size() - 1; i >= 0; i--) {
    if (destroyee == cColoursCycle[i]) {
      cColoursCycle.erase(cColoursCycle.begin() + i);
      mChanged = true;
    }
  }
  if (mChanged) {
    for (unsigned int i = 0; i < cInstances.size(); i++) {
      cInstances[i]->refresh();
    }
  }
}
