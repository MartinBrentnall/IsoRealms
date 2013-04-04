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
#include "SpindizzyJewelModelFactory.h"

I3DModel* SpindizzyJewelModelFactory::createModel(Vertex* location, float scale) {
  SpindizzyJewelModel* mJewelModel = new SpindizzyJewelModel(location, scale, &cColoursCycle, &cColourFrame);
  cInstances.push_back(mJewelModel);
  return mJewelModel;
}

void SpindizzyJewelModelFactory::destroyModel(I3DModel* model) {
  delete model;
}

void SpindizzyJewelModelFactory::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("frameColour", resourceLocator->getPath(cColourFrame));
  for (unsigned int i = 0; cColoursCycle.size(); i++) {
    DOMNodeWriter* mNode = node->addBranch("ColourCycle");
    mNode->addAttribute("colour", resourceLocator->getPath(cColoursCycle[i]));
  }
}

void SpindizzyJewelModelFactory::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mFrameColour = node->getAttribute("frameColour");
  cColourFrame = resourceAccessor->getColour(mFrameColour);
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

void SpindizzyJewelModelFactory::resourceChanged(IColour* colour) {
  // No action necessary
}

void SpindizzyJewelModelFactory::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
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
