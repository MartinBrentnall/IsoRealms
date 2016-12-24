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

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Texture/ITexture.h>
#include <IsoRealms/Resources/Script/IScriptCall.h>

#include "IWallPattern.h"
#include "TextureRotation.h"
#include "WallType.h"
#include "WallPatternCap.h"
#include "WallPatternTile.h"

class BlockTypeProperties {
  private:
  IScriptCall* cContactScript;
  IScriptCall* cImpactScript;
  float cSurfaceFriction;
  float cSurfaceGrip;
  float cSurfaceBounce;
  bool cRespawnAllowed;
  ITexture* cSurfaceTexture;
  ITexture* cSurfaceSplitNETexture;
  ITexture* cSurfaceSplitNWTexture;
  IWallPattern* cWestWallPattern;
  IWallPattern* cEastWallPattern;
  IWallPattern* cSouthWallPattern;
  IWallPattern* cNorthWallPattern;
  TextureRotation cSurfaceRotation;
  WallType cWallType;

  void saveTexture(DOMNodeWriter*, const std::string&, ITexture*, IResourceLocator*, bool = false);
  void saveTextureFloor(DOMNodeWriter*, const std::string&, ITexture*, IResourceLocator*, TextureRotation);
  
  IWallPattern* configureWall(DOMNodeWrapper*, IResourceAccessor*);
  
  public:
  BlockTypeProperties();
  
  void configure(DOMNodeWrapper*, IResourceAccessor*);
  
  void executeContactScript();
  void executeImpactScript();
  float getSurfaceFriction();
  float getSurfaceGrip();
  float getSurfaceBounce();
  bool isRespawnAllowed();
  ITexture** getSurfaceTexture();
  ITexture** getSplitNETexture();
  ITexture** getSplitNWTexture();
  IWallPattern* getWestWallPattern();
  IWallPattern* getEastWallPattern();
  IWallPattern* getSouthWallPattern();
  IWallPattern* getNorthWallPattern();
  TextureRotation getSurfaceRotation();
  void save(DOMNodeWriter*, IResourceLocator*);
  
  bool resourcePendingDestruction(ITexture*, ITexture*);
  bool contains(ITexture*);
};

#endif
