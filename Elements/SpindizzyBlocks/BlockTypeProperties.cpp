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
  cSurfaceRotation = STRAIGHT;
  cWallType = TILED;
}

void BlockTypeProperties::configure(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cSurfaceFriction = node->getFloatAttribute("friction");
  cSurfaceGrip = node->getFloatAttribute("grip");
  cRespawnAllowed = node->getBooleanAttribute("respawnAllowed");
  cSurfaceBounce = node->getFloatAttribute("bounce");
  std::string mWallType = node->getAttribute("wallType");
  cWallType = mWallType == "capped" ? CAPPED
            :                         TILED;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ContactScript") {
      cContactScript = runtimeContext->getLuaScript(mNode);
    } else if (mValueAsString == "ImpactScript") {
      cImpactScript = runtimeContext->getLuaScript(mNode);
    } else if (mValueAsString == "Texture") {
      std::string mApplyTo = mNode->getAttribute("type");
      ITexture* mTexture = runtimeContext->getTexture(mNode);
      if (mApplyTo == "Surface") {
        cSurfaceTexture = mTexture;
        std::string mTextureRotation = mNode->getAttribute("rotate");
        cSurfaceRotation = mTextureRotation == "Left"    ? LEFT
                         : mTextureRotation == "Right"   ? RIGHT
                         : mTextureRotation == "Reverse" ? REVERSE
                         :                                 STRAIGHT;
      } else if (mApplyTo == "NESplitSurface") {
        cSurfaceSplitNETexture = mTexture;
      } else if (mApplyTo == "NWSplitSurface") {
        cSurfaceSplitNWTexture = mTexture;
      } else if (mApplyTo == "WallWest") {
        cWestWallTexture = mTexture;
      } else if (mApplyTo == "WallEast") {
        cEastWallTexture = mTexture;
      } else if (mApplyTo == "WallSouth") {
        cSouthWallTexture = mTexture;
      } else if (mApplyTo == "WallNorth") {
        cNorthWallTexture = mTexture;
      } else if (mApplyTo == "WallWestTop") {
        cWestWallTextureTop = mTexture;
      } else if (mApplyTo == "WallEastTop") {
        cEastWallTextureTop = mTexture;
      } else if (mApplyTo == "WallSouthTop") {
        cSouthWallTextureTop = mTexture;
      } else if (mApplyTo == "WallNorthTop") {
        cNorthWallTextureTop = mTexture;
      } else if (mApplyTo == "WallWestBottom") {
        cWestWallTextureBottom = mTexture;
        std::string mFlip = mNode->getAttribute("flip");
        cWestBottomFlip = mFlip == "true" ? true : false;
      } else if (mApplyTo == "WallEastBottom") {
        cEastWallTextureBottom = mTexture;
        std::string mFlip = mNode->getAttribute("flip");
        cEastBottomFlip = mFlip == "true" ? true : false;
      } else if (mApplyTo == "WallSouthBottom") {
        cSouthWallTextureBottom = mTexture;
        std::string mFlip = mNode->getAttribute("flip");
        cSouthBottomFlip = mFlip == "true" ? true : false;
      } else if (mApplyTo == "WallNorthBottom") {
        cNorthWallTextureBottom = mTexture;
        std::string mFlip = mNode->getAttribute("flip");
        cNorthBottomFlip = mFlip == "true" ? true : false;
      } else {
        // TODO: Throw
        std::cout << "Unknown texture type" << std::endl;
        exit(1);
      }
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

ITexture* BlockTypeProperties::getSurfaceTexture() {
  return cSurfaceTexture;
}

ITexture* BlockTypeProperties::getSplitNETexture() {
  return cSurfaceSplitNETexture != NULL ? cSurfaceSplitNETexture : getSurfaceTexture();
}

ITexture* BlockTypeProperties::getSplitNWTexture() {
  return cSurfaceSplitNETexture != NULL ? cSurfaceSplitNETexture : getSurfaceTexture();
}

WallType BlockTypeProperties::getWallType() {
  return cWallType;
}

ITexture* BlockTypeProperties::getWestWallTexture() {
  return cWestWallTexture;
}

ITexture* BlockTypeProperties::getEastWallTexture() {
  return cEastWallTexture;
}

ITexture* BlockTypeProperties::getSouthWallTexture() {
  return cSouthWallTexture;
}

ITexture* BlockTypeProperties::getNorthWallTexture() {
  return cNorthWallTexture;
}

ITexture* BlockTypeProperties::getWestWallTextureTop() {
  return cWestWallTextureTop;
}

ITexture* BlockTypeProperties::getEastWallTextureTop() {
  return cEastWallTextureTop;
}

ITexture* BlockTypeProperties::getSouthWallTextureTop() {
  return cSouthWallTextureTop;
}

ITexture* BlockTypeProperties::getNorthWallTextureTop() {
  return cNorthWallTextureTop;
}

ITexture* BlockTypeProperties::getWestWallTextureBottom() {
  return cWestWallTextureBottom;
}

ITexture* BlockTypeProperties::getEastWallTextureBottom() {
  return cEastWallTextureBottom;
}

ITexture* BlockTypeProperties::getSouthWallTextureBottom() {
  return cSouthWallTextureBottom;
}

ITexture* BlockTypeProperties::getNorthWallTextureBottom() {
  return cNorthWallTextureBottom;
}

bool BlockTypeProperties::isWestWallBottomFlipped() {
  return cWestBottomFlip;
}

bool BlockTypeProperties::isEastWallBottomFlipped() {
  return cEastBottomFlip;
}

bool BlockTypeProperties::isSouthWallBottomFlipped() {
  return cSouthBottomFlip;
}

bool BlockTypeProperties::isNorthWallBottomFlipped() {
  return cNorthBottomFlip;
}
  
TextureRotation BlockTypeProperties::getSurfaceRotation() {
  return cSurfaceRotation;
}

void BlockTypeProperties::save(DOMNodeWriter* node) {
  node->addAttribute("friction", cSurfaceFriction);
  node->addAttribute("grip", cSurfaceGrip);
  node->addAttribute("bounce", cSurfaceBounce);
  node->addAttribute("respawnAllowed", cRespawnAllowed ? "true" : "false");
/*  cImpactScript->save(node, "ImpactScript");
  cContactScript->save(node, "ContactScript");*/
  cSurfaceTexture->save(node, "Surface");
  cSurfaceSplitNETexture->save(node, "NESplitSurface");
  cSurfaceSplitNWTexture->save(node, "NWSplitSurface");
  cWestWallTexture->save(node, "WallWest");
  cEastWallTexture->save(node, "WallEast");
  cSouthWallTexture->save(node, "WallSouth");
  cNorthWallTexture->save(node, "WallNorth");
  
}
