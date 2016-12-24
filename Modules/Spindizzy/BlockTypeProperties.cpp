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
  cContactScript = nullptr;
  cImpactScript = nullptr;
  cSurfaceFriction = 0.001f;
  cSurfaceGrip = 1.0f;
  cSurfaceBounce = 0.0f;
  cRespawnAllowed = true;
  cSurfaceRotation = STRAIGHT;
  cWallType = TILED;
  
  cSurfaceTexture         = nullptr;
  cSurfaceSplitNETexture  = nullptr;
  cSurfaceSplitNWTexture  = nullptr;
  cWestWallPattern        = nullptr;
  cEastWallPattern        = nullptr;
  cSouthWallPattern       = nullptr;
  cNorthWallPattern       = nullptr;
}

IWallPattern* BlockTypeProperties::configureWall(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  std::string mPatternType = node->getAttribute("type");
  return mPatternType == "tile" ? static_cast<IWallPattern*>(new WallPatternTile(node, resourceAccessor))
       : mPatternType == "cap"  ? static_cast<IWallPattern*>(new WallPatternCap( node, resourceAccessor))
       :                          nullptr;
}

void BlockTypeProperties::configure(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  cSurfaceFriction = node->getFloatAttribute("friction");
  cSurfaceGrip = node->getFloatAttribute("grip");
  cRespawnAllowed = node->getBooleanAttribute("respawnAllowed");
  cSurfaceBounce = node->getFloatAttribute("bounce");
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ContactScript") {
      cContactScript = resourceAccessor->getScriptCall(mNode);
    } else if (mValueAsString == "ImpactScript") {
      cImpactScript = resourceAccessor->getScriptCall(mNode);
    } else if (mValueAsString == "WallWest") {
      cWestWallPattern = configureWall(mNode, resourceAccessor);
    } else if (mValueAsString == "WallEast") {
      cEastWallPattern = configureWall(mNode, resourceAccessor);
    } else if (mValueAsString == "WallSouth") {
      cSouthWallPattern = configureWall(mNode, resourceAccessor);
    } else if (mValueAsString == "WallNorth") {
      cNorthWallPattern = configureWall(mNode, resourceAccessor);
    } else if (mValueAsString == "Texture") {
      std::string mApplyTo = mNode->getAttribute("type");
      std::string mTexturePath = mNode->getAttribute("name");
      ITexture* mTexture = resourceAccessor->getTexture(mTexturePath);
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
      } else {
        // TODO: Throw
        std::cout << "Unknown texture type \"" << mApplyTo << "\"" << std::endl;
        exit(1);
      }
    }
  }
}

void BlockTypeProperties::executeContactScript() {
  if (cContactScript != nullptr) {
    cContactScript->execute();
  }
}

void BlockTypeProperties::executeImpactScript() {
  if (cImpactScript != nullptr) {
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

ITexture** BlockTypeProperties::getSurfaceTexture() {
  return &cSurfaceTexture;
}

ITexture** BlockTypeProperties::getSplitNETexture() {
  return cSurfaceSplitNETexture != nullptr ? &cSurfaceSplitNETexture : getSurfaceTexture();
}

ITexture** BlockTypeProperties::getSplitNWTexture() {
  return cSurfaceSplitNETexture != nullptr ? &cSurfaceSplitNETexture : getSurfaceTexture();
}

IWallPattern* BlockTypeProperties::getWestWallPattern() {
  return cWestWallPattern;
}

IWallPattern* BlockTypeProperties::getEastWallPattern() {
  return cEastWallPattern;
}

IWallPattern* BlockTypeProperties::getSouthWallPattern() {
  return cSouthWallPattern;
}

IWallPattern* BlockTypeProperties::getNorthWallPattern() {
  return cNorthWallPattern;
}

TextureRotation BlockTypeProperties::getSurfaceRotation() {
  return cSurfaceRotation;
}

void BlockTypeProperties::saveTexture(DOMNodeWriter* node, const std::string& type, ITexture* texture, IResourceLocator* resourceLocator, bool flip) {
  if (texture != nullptr) {
    DOMNodeWriter* mTextureNode = node->addBranch("Texture");
    mTextureNode->addAttribute("type", type);
    mTextureNode->addAttribute("name", resourceLocator->getPath(texture));
    if (flip) {
      mTextureNode->addAttribute("flip", "true");
    }
  }
}

void BlockTypeProperties::saveTextureFloor(DOMNodeWriter* node, const std::string& type, ITexture* texture, IResourceLocator* resourceLocator, TextureRotation rotation) {
  if (texture != nullptr) {
    DOMNodeWriter* mTextureNode = node->addBranch("Texture");
    mTextureNode->addAttribute("type", type);
    mTextureNode->addAttribute("name", resourceLocator->getPath(texture));
    if (rotation != STRAIGHT) {
      mTextureNode->addAttribute("rotate", cSurfaceRotation == LEFT  ? "Left" 
                                         : cSurfaceRotation == RIGHT ? "Right"
                                         :                             "Reverse");
    }
  }
}

void BlockTypeProperties::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("friction", cSurfaceFriction);
  node->addAttribute("grip", cSurfaceGrip);
  node->addAttribute("bounce", cSurfaceBounce);
  node->addAttribute("respawnAllowed", cRespawnAllowed ? "true" : "false");
  resourceLocator->saveScript(node, "ImpactScript", cImpactScript);
  resourceLocator->saveScript(node, "ContactScript", cContactScript);
  cNorthWallPattern->save(node, resourceLocator);
  cSouthWallPattern->save(node, resourceLocator);
  cWestWallPattern->save(node, resourceLocator);
  cEastWallPattern->save(node, resourceLocator);
//   if (cWallType == TILED) {
//     node->addAttribute("wallType", "tiled");
//     saveTextureFloor(node, "Surface", cSurfaceTexture, resourceLocator, cSurfaceRotation);
//     saveTexture(node, "WallWest",  cWestWallTexture,  resourceLocator);
//     saveTexture(node, "WallEast",  cEastWallTexture,  resourceLocator);
//     saveTexture(node, "WallSouth", cSouthWallTexture, resourceLocator);
//     saveTexture(node, "WallNorth", cNorthWallTexture, resourceLocator);
//   } else if (cWallType == CAPPED) {
//     node->addAttribute("wallType", "capped");
//     saveTextureFloor(node, "Surface", cSurfaceTexture, resourceLocator, cSurfaceRotation);
//     saveTexture(node, "NESplitSurface",  cSurfaceSplitNETexture,  resourceLocator);
//     saveTexture(node, "NWSplitSurface",  cSurfaceSplitNWTexture,  resourceLocator);
//     saveTexture(node, "WallWest",        cWestWallTexture,        resourceLocator);
//     saveTexture(node, "WallWestTop",     cWestWallTextureTop,     resourceLocator);
//     saveTexture(node, "WallWestBottom",  cWestWallTextureBottom,  resourceLocator, cWestBottomFlip);
//     saveTexture(node, "WallEast",        cEastWallTexture,        resourceLocator);
//     saveTexture(node, "WallEastTop",     cEastWallTextureTop,     resourceLocator);
//     saveTexture(node, "WallEastBottom",  cEastWallTextureBottom,  resourceLocator, cEastBottomFlip);
//     saveTexture(node, "WallSouth",       cSouthWallTexture,       resourceLocator);
//     saveTexture(node, "WallSouthTop",    cSouthWallTextureTop,    resourceLocator);
//     saveTexture(node, "WallSouthBottom", cSouthWallTextureBottom, resourceLocator, cSouthBottomFlip);
//     saveTexture(node, "WallNorth",       cNorthWallTexture,       resourceLocator);
//     saveTexture(node, "WallNorthTop",    cNorthWallTextureTop,    resourceLocator);
//     saveTexture(node, "WallNorthBottom", cNorthWallTextureBottom, resourceLocator, cNorthBottomFlip);
//   }
}

bool BlockTypeProperties::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  bool mTexturesReplaced = false;
  mTexturesReplaced |= Utils::replaceTexture(cSurfaceTexture,         destroyee, replacement);
  mTexturesReplaced |= Utils::replaceTexture(cSurfaceSplitNETexture,  destroyee, replacement);
  mTexturesReplaced |= Utils::replaceTexture(cSurfaceSplitNWTexture,  destroyee, replacement);
  mTexturesReplaced |= cNorthWallPattern->resourcePendingDestruction(destroyee, replacement);
  mTexturesReplaced |= cSouthWallPattern->resourcePendingDestruction(destroyee, replacement);
  mTexturesReplaced |= cWestWallPattern->resourcePendingDestruction(destroyee, replacement);
  mTexturesReplaced |= cEastWallPattern->resourcePendingDestruction(destroyee, replacement);
  return mTexturesReplaced;
}

bool BlockTypeProperties::contains(ITexture* texture) {
  return cSurfaceTexture         == texture
      || cSurfaceSplitNETexture  == texture
      || cSurfaceSplitNWTexture  == texture
      || cNorthWallPattern->contains(texture)
      || cSouthWallPattern->contains(texture)
      || cWestWallPattern->contains(texture)
      || cEastWallPattern->contains(texture);
}
