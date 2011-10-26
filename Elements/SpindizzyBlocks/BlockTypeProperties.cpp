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
  cContactScript = Script::getDummy();
  cImpactScript = Script::getDummy();
  cSurfaceFriction = 0.001f;
  cSurfaceGrip = 1.0f;
  cSurfaceBounce = 0.0f;
  cRespawnAllowed = true;
}

void BlockTypeProperties::configure(DOMNodeWrapper* node, ICommandRegistry* commandRegistry, ISpindizzyTextureSet** textureSet) {
  cTextureSet = textureSet;
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
    } else if (mValueAsString == "Texture") {
      std::string mApplyTo = mNode->getAttribute("type");
      std::string mTextureName = mNode->getAttribute("name");
      if (mApplyTo == "Surface") {
        cSurfaceTexture = mTextureName;
      } else if (mApplyTo == "NESplitSurface") {
        cSurfaceSplitNETexture = mTextureName;
      } else if (mApplyTo == "NWSplitSurface") {
        cSurfaceSplitNWTexture = mTextureName;
      } else if (mApplyTo == "WallWest") {
        cWestWallTexture = mTextureName;
      } else if (mApplyTo == "WallEast") {
        cEastWallTexture = mTextureName;
      } else if (mApplyTo == "WallSouth") {
        cSouthWallTexture = mTextureName;
      } else if (mApplyTo == "WallNorth") {
        cNorthWallTexture = mTextureName;
      } else {
        // TODO: Throw
        std::cout << "Unknown texture type" << std::endl;
        exit(1);
      }
    }
  }
}

void BlockTypeProperties::executeContactScript() {
  cContactScript->execute();
}

void BlockTypeProperties::executeImpactScript() {
  cImpactScript->execute();
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

ISpindizzyTexture* BlockTypeProperties::getSurfaceTexture() {
  return (*cTextureSet)->getTexture(cSurfaceTexture);
}

ISpindizzyTexture* BlockTypeProperties::getSplitNETexture() {
  ISpindizzyTexture* mSplitTexture = (*cTextureSet)->getTexture(cSurfaceSplitNETexture);
  return mSplitTexture != NULL ? mSplitTexture : getSurfaceTexture();
}

ISpindizzyTexture* BlockTypeProperties::getSplitNWTexture() {
  ISpindizzyTexture* mSplitTexture = (*cTextureSet)->getTexture(cSurfaceSplitNWTexture);
  return mSplitTexture != NULL ? mSplitTexture : getSurfaceTexture();
}

ISpindizzyTexture* BlockTypeProperties::getWestWallTexture() {
  return (*cTextureSet)->getTexture(cWestWallTexture);
}

ISpindizzyTexture* BlockTypeProperties::getEastWallTexture() {
  return (*cTextureSet)->getTexture(cEastWallTexture);
}

ISpindizzyTexture* BlockTypeProperties::getSouthWallTexture() {
  return (*cTextureSet)->getTexture(cSouthWallTexture);
}

ISpindizzyTexture* BlockTypeProperties::getNorthWallTexture() {
  return (*cTextureSet)->getTexture(cNorthWallTexture);
}

void BlockTypeProperties::saveTexture(DOMNodeWriter* node, const std::string& type, const std::string& name) {
  if (name != "") {
    DOMNodeWriter* mTextureNode = node->addBranch("Texture");
    mTextureNode->addAttribute("type", type);
    mTextureNode->addAttribute("name", name);
  }    
}

void BlockTypeProperties::save(DOMNodeWriter* node) {
  node->addAttribute("friction", cSurfaceFriction);
  node->addAttribute("grip", cSurfaceGrip);
  node->addAttribute("bounce", cSurfaceBounce);
  node->addAttribute("respawnAllowed", cRespawnAllowed ? "true" : "false");
  cImpactScript->save(node, "ImpactScript");
  cContactScript->save(node, "ContactScript");
  saveTexture(node, "Surface", cSurfaceTexture);
  saveTexture(node, "NESplitSurface", cSurfaceSplitNETexture);
  saveTexture(node, "NWSplitSurface", cSurfaceSplitNWTexture);
  saveTexture(node, "WallWest", cWestWallTexture);
  saveTexture(node, "WallEast", cEastWallTexture);
  saveTexture(node, "WallSouth", cSouthWallTexture);
  saveTexture(node, "WallNorth", cNorthWallTexture);
  
}
