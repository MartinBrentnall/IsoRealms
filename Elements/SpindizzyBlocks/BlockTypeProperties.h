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
#ifndef BLOCK_TYPE_PROPERTIES_H
#define BLOCK_TYPE_PROPERTIES_H

#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/IRuntimeContext.h>
#include <IsoRealms/ITexture.h>
#include <IsoRealms/IScript.h>

#include "TextureRotation.h"
#include "WallType.h"

class BlockTypeProperties {
  private:
  IScript* cContactScript;
  IScript* cImpactScript;
  float cSurfaceFriction;
  float cSurfaceGrip;
  float cSurfaceBounce;
  bool cRespawnAllowed;
  ITexture* cSurfaceTexture;
  ITexture* cSurfaceSplitNETexture;
  ITexture* cSurfaceSplitNWTexture;
  ITexture* cWestWallTexture;
  ITexture* cEastWallTexture;
  ITexture* cSouthWallTexture;
  ITexture* cNorthWallTexture;
  ITexture* cWestWallTextureTop;
  ITexture* cEastWallTextureTop;
  ITexture* cSouthWallTextureTop;
  ITexture* cNorthWallTextureTop;
  ITexture* cWestWallTextureBottom;
  ITexture* cEastWallTextureBottom;
  ITexture* cSouthWallTextureBottom;
  ITexture* cNorthWallTextureBottom;
  bool cWestBottomFlip;
  bool cEastBottomFlip;
  bool cSouthBottomFlip;
  bool cNorthBottomFlip;
  TextureRotation cSurfaceRotation;
  WallType cWallType;
  
  public:
  BlockTypeProperties();
  
  void configure(DOMNodeWrapper*, IRuntimeContext*);
  
  void executeContactScript();
  void executeImpactScript();
  float getSurfaceFriction();
  float getSurfaceGrip();
  float getSurfaceBounce();
  bool isRespawnAllowed();
  WallType getWallType();
  ITexture* getSurfaceTexture();
  ITexture* getSplitNETexture();
  ITexture* getSplitNWTexture();
  ITexture* getWestWallTexture();
  ITexture* getEastWallTexture();
  ITexture* getSouthWallTexture();
  ITexture* getNorthWallTexture();
  ITexture* getWestWallTextureTop();
  ITexture* getEastWallTextureTop();
  ITexture* getSouthWallTextureTop();
  ITexture* getNorthWallTextureTop();
  ITexture* getWestWallTextureBottom();
  ITexture* getEastWallTextureBottom();
  ITexture* getSouthWallTextureBottom();
  ITexture* getNorthWallTextureBottom();
  bool isWestWallBottomFlipped();
  bool isEastWallBottomFlipped();
  bool isSouthWallBottomFlipped();
  bool isNorthWallBottomFlipped();
  TextureRotation getSurfaceRotation();
  void save(DOMNodeWriter*);
};

#endif
