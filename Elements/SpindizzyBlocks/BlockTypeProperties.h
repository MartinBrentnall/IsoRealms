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
#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/Script.h>

#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"

class BlockTypeProperties {
  private:
  Script* cContactScript;
  Script* cImpactScript;
  float cSurfaceFriction;
  float cSurfaceGrip;
  float cSurfaceBounce;
  bool cRespawnAllowed;
  ISpindizzyTextureSet** cTextureSet;
  std::string cSurfaceTexture;
  std::string cSurfaceSplitNETexture;
  std::string cSurfaceSplitNWTexture;
  std::string cWestWallTexture;
  std::string cEastWallTexture;
  std::string cSouthWallTexture;
  std::string cNorthWallTexture;
  
  public:
  BlockTypeProperties();
  
  void configure(DOMNodeWrapper*, ICommandRegistry*, ISpindizzyTextureSet**);
  
  void executeContactScript();
  void executeImpactScript();
  float getSurfaceFriction();
  float getSurfaceGrip();
  float getSurfaceBounce();
  bool isRespawnAllowed();
  ISpindizzyTexture* getSurfaceTexture();
  ISpindizzyTexture* getSplitNETexture();
  ISpindizzyTexture* getSplitNWTexture();
  ISpindizzyTexture* getWestWallTexture();
  ISpindizzyTexture* getEastWallTexture();
  ISpindizzyTexture* getSouthWallTexture();
  ISpindizzyTexture* getNorthWallTexture();
};

#endif
