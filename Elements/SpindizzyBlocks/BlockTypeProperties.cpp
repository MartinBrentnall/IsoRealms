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
  cSurfaceRotation = STRAIGHT;
  cWallType = TILED;
}

void BlockTypeProperties::configure(DOMNodeWrapper* node, IRuntimeContext* runtimeContext, ITextureSet** textureSet) {
  cTextureSet = textureSet;
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
      cContactScript = runtimeContext->getScript(mNode);
    } else if (mValueAsString == "ImpactScript") {
      cImpactScript = runtimeContext->getScript(mNode);
    } else if (mValueAsString == "Texture") {
      std::string mApplyTo = mNode->getAttribute("type");
      std::string mTextureName = mNode->getAttribute("name");
      if (mApplyTo == "Surface") {
        cSurfaceTexture = mTextureName;
        std::string mTextureRotation = mNode->getAttribute("rotate");
        cSurfaceRotation = mTextureRotation == "Left"    ? LEFT
                         : mTextureRotation == "Right"   ? RIGHT
                         : mTextureRotation == "Reverse" ? REVERSE
                         :                                 STRAIGHT;
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
      } else if (mApplyTo == "WallWestTop") {
        cWestWallTextureTop = mTextureName;
      } else if (mApplyTo == "WallEastTop") {
        cEastWallTextureTop = mTextureName;
      } else if (mApplyTo == "WallSouthTop") {
        cSouthWallTextureTop = mTextureName;
      } else if (mApplyTo == "WallNorthTop") {
        cNorthWallTextureTop = mTextureName;
      } else if (mApplyTo == "WallWestBottom") {
        cWestWallTextureBottom = mTextureName;
        std::string mFlip = mNode->getAttribute("flip");
        cWestBottomFlip = mFlip == "true" ? true : false;
      } else if (mApplyTo == "WallEastBottom") {
        cEastWallTextureBottom = mTextureName;
        std::string mFlip = mNode->getAttribute("flip");
        cEastBottomFlip = mFlip == "true" ? true : false;
      } else if (mApplyTo == "WallSouthBottom") {
        cSouthWallTextureBottom = mTextureName;
        std::string mFlip = mNode->getAttribute("flip");
        cSouthBottomFlip = mFlip == "true" ? true : false;
      } else if (mApplyTo == "WallNorthBottom") {
        cNorthWallTextureBottom = mTextureName;
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

ITexture* BlockTypeProperties::getSurfaceTexture() {
  return (*cTextureSet)->getTexture(cSurfaceTexture);
}

ITexture* BlockTypeProperties::getSplitNETexture() {
  ITexture* mSplitTexture = (*cTextureSet)->getTexture(cSurfaceSplitNETexture);
  return mSplitTexture != NULL ? mSplitTexture : getSurfaceTexture();
}

ITexture* BlockTypeProperties::getSplitNWTexture() {
  ITexture* mSplitTexture = (*cTextureSet)->getTexture(cSurfaceSplitNWTexture);
  return mSplitTexture != NULL ? mSplitTexture : getSurfaceTexture();
}

WallType BlockTypeProperties::getWallType() {
  return cWallType;
}

ITexture* BlockTypeProperties::getWestWallTexture() {
  return (*cTextureSet)->getTexture(cWestWallTexture);
}

ITexture* BlockTypeProperties::getEastWallTexture() {
  return (*cTextureSet)->getTexture(cEastWallTexture);
}

ITexture* BlockTypeProperties::getSouthWallTexture() {
  return (*cTextureSet)->getTexture(cSouthWallTexture);
}

ITexture* BlockTypeProperties::getNorthWallTexture() {
  return (*cTextureSet)->getTexture(cNorthWallTexture);
}

ITexture* BlockTypeProperties::getWestWallTextureTop() {
  return (*cTextureSet)->getTexture(cWestWallTextureTop);
}

ITexture* BlockTypeProperties::getEastWallTextureTop() {
  return (*cTextureSet)->getTexture(cEastWallTextureTop);
}

ITexture* BlockTypeProperties::getSouthWallTextureTop() {
  return (*cTextureSet)->getTexture(cSouthWallTextureTop);
}

ITexture* BlockTypeProperties::getNorthWallTextureTop() {
  return (*cTextureSet)->getTexture(cNorthWallTextureTop);
}

ITexture* BlockTypeProperties::getWestWallTextureBottom() {
  return (*cTextureSet)->getTexture(cWestWallTextureBottom);
}

ITexture* BlockTypeProperties::getEastWallTextureBottom() {
  return (*cTextureSet)->getTexture(cEastWallTextureBottom);
}

ITexture* BlockTypeProperties::getSouthWallTextureBottom() {
  return (*cTextureSet)->getTexture(cSouthWallTextureBottom);
}

ITexture* BlockTypeProperties::getNorthWallTextureBottom() {
  return (*cTextureSet)->getTexture(cNorthWallTextureBottom);
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
