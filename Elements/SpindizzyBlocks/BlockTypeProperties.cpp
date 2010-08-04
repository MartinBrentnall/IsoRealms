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
#include "BlockTypeProperties.h"

BlockTypeProperties::BlockTypeProperties() {
  cContactScript = NULL;
  cImpactScript = NULL;
  cSurfaceFriction = 0.001f;
  cSurfaceGrip = 1.0f;
  cSurfaceBounce = 0.0f;
  cRespawnAllowed = true;
}

void BlockTypeProperties::configure(DOMNodeWrapper* node, ICommandRegistry* commandRegistry) {
  cSurfaceFriction = node->getFloatAttribute("friction");
  cSurfaceGrip = node->getFloatAttribute("grip");
  cRespawnAllowed = node->getBooleanAttribute("respawnAllowed");
  cSurfaceBounce = node->getFloatAttribute("bounce");
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ContactScript") {
      cContactScript = commandRegistry->getScript(mNode);
    } else if (mValueAsString == "ImpactScript") {
      cImpactScript = commandRegistry->getScript(mNode);
    }
  }
}

void BlockTypeProperties::executeContactScript() {
  if (cContactScript != NULL) {
    cContactScript->execute();
  }
}

void BlockTypeProperties::executeImpactScript() {
  if (cImpactScript != NULL) {
    cImpactScript->execute();
  }
}

float BlockTypeProperties::getSurfaceFriction() {
  return cSurfaceFriction;
}

float BlockTypeProperties::getSurfaceGrip() {
  return cSurfaceGrip;
}

float BlockTypeProperties::getSurfaceBounce() {
  return cSurfaceBounce;
}

bool BlockTypeProperties::isRespawnAllowed() {
  return cRespawnAllowed;
}
