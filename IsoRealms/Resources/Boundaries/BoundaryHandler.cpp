/*
 * Copyright 2015 Martin Brentnall
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
#include "BoundaryHandler.h"

BoundaryHandler::BoundaryHandler(DOMNodeWrapper* node, IResourceAccessor* resources, IBoundaries* boundaries) {
  cBoundaries = boundaries;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "EnteredScript") {
      cScriptEntered = resources->getScriptCall(mNode);
    } else if (mValueAsString == "ExitedScript") {
      cScriptExited = resources->getScriptCall(mNode);
    } else {
      // TODO: Throw
    }
  }
}

void BoundaryHandler::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  std::string mBoundariesPath = resourceLocator->getPath(cBoundaries);
  node->addAttribute("name", mBoundariesPath);
  DOMNodeWriter* mEnteredScriptNode = node->addBranch("EnteredScript");
  cScriptEntered->save(mEnteredScriptNode, resourceLocator);
  DOMNodeWriter* mExitedScriptNode = node->addBranch("ExitedScript");
  cScriptExited->save(mExitedScriptNode, resourceLocator);
}
  
void BoundaryHandler::notifyMovement(IBoundaryPenetrator* client, Vertex& start, Vertex& end) {
  cBoundaries->notifyMovement(client, start, end, this);
}

void BoundaryHandler::notifyAppearance(IBoundaryPenetrator* client, Vertex& location) {
  cBoundaries->notifyAppearance(client, location, this);
}

void BoundaryHandler::notifyDisappearance(IBoundaryPenetrator* client, Vertex& location) {
  cBoundaries->notifyDisappearance(client, location, this);
}

void BoundaryHandler::entered() {
  cScriptEntered->execute();
}

void BoundaryHandler::exited() {
  cScriptExited->execute();
}
