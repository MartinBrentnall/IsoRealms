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
#ifndef RESOURCE_TEXTURES_SPINDIZZY_BLOCKS_C64_H
#define RESOURCE_TEXTURES_SPINDIZZY_BLOCKS_C64_H

#include <cmath>
#include <map>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/IResourceInstanceListener.h>
#include <IsoRealms/Resources/Texture/ITexture.h>
#include <IsoRealms/Resources/Texture/Texture.h>

#include "ICameraSupplier.h"

class ResourceTexturesSpindizzyBlocksC64:public IResource,
                                         public IResourceUseListener<IColour>,
                                         public ICameraAngleChangeListener {
  private:
  static const float TILE_SIZE;
  static const float SWITCH_SQUARE_OUTER;
  static const float SWITCH_SQUARE_INNER;
  static const float SWITCH_SQUARE_TRIANGLE_INNER;
  static const float SWITCH_DIAMOND_OUTER;
  static const float SWITCH_DIAMOND_INNER;
  static const float SWITCH_CIRCLE_OUTER;
  static const float SWITCH_CIRCLE_INNER;
  static const float ICE_EDGE_WIDTH;
  static const float ARROW_SIZE;
  static const float ARROW_LINE_WIDTH;
  static const float CIRCLE_RESOLUTION;

  static const std::string PLAIN;
  static const std::string PLAIN_SPLIT;
  static const std::string ICE_WATER;
  static const std::string ARROW;
  static const std::string SWITCH_CIRCLE_BOTH;
  static const std::string SWITCH_CIRCLE_ONE;
  static const std::string SWITCH_CIRCLE_NONE;
  static const std::string SWITCH_SQUARE_BOTH;
  static const std::string SWITCH_SQUARE_ONE;
  static const std::string SWITCH_SQUARE_NONE;
  static const std::string SWITCH_DIAMOND_BOTH;
  static const std::string SWITCH_DIAMOND_ONE;
  static const std::string SWITCH_DIAMOND_NONE;
  static const std::string TRAMPOLINE;
  static const std::string WALL_ICE;
  static const std::string WALL_MIXED_CAP;
  static const std::string WALL_MIXED_MIDDLE;
  static const std::string WALL_PLAIN_CAP;
  static const std::string WALL_PLAIN_MIDDLE;

  std::map<std::string, Texture*> cTextures; 
  std::map<std::string, GLuint> cTextureIDs;

  IColour* cFloorColour;
  IColour* cWallColour;
  IColour* cGridColour;
  IColour* cBackgroundColour;

  void initialise();
  
  float getGridWallLuminanceAdjustment();
  
  void clear(IColour*);
  void renderSquare(float, IColour*);
  void renderDiamond(float, IColour*);
  void renderCircle(float, IColour*);
  void renderRectangle(float, float, float, float, IColour*);
  void renderTile(IColour*);
  void renderIce();
  void renderPlain();
  void renderSplitPlain();
  void renderTrampoline();
  void renderSwitchSquare(float);
  void renderSwitchSquareHalf(float);
  void renderSwitchSquareBoth(float);
  void renderSwitchDiamond(float);
  void renderSwitchDiamondHalf(float);
  void renderSwitchDiamondBoth();
  void renderSwitchCircle(float);
  void renderSwitchCircleBoth();
  void renderSwitchCircleHalf(float);
  void renderArrow();
  void renderWallMiddle(float);
  void renderWallCap(float);
  void renderIceWall();

  void generateAngledTextures(float);
  void generateTextures();
  void destroyTextures();

  public:
  ResourceTexturesSpindizzyBlocksC64(ICameraSupplier*, DOMNodeWrapper*, IResourceRegistry*);
  
  std::string stripMemberName(const std::string&);
  
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  IColour* getFloorColour();
  IColour* getWallColour();
  IColour* getGridColour();
  IColour* getDetailColour();
  void setFloorColour(IColour*);
  void setWallColour(IColour*);
  void setGridColour(IColour*);
  void setDetailColour(IColour*);
  std::vector<ITexture*> getResources();
  std::vector<std::string> getResourceNames();
  ITexture* getResource(const std::string&);
  
  /*******************************\
   * Implements IPaletteListener *
  \*******************************/
  void resourceChanged(IColour*);
  void resourcePendingDestruction(IColour*, IColour*);
  bool contains(ITexture*);
  void deleteResources(IResourceInstanceListener<ITexture>*);
  void addTextures(IResourceRegistry*, const std::string&);

  void save(DOMNodeWriter*, IResourceLocator*);

  /*****************************************\
   * Implements ICameraAngleChangeListener *
  \*****************************************/
  void cameraAngleChanged(float);
  
  virtual ~ResourceTexturesSpindizzyBlocksC64();
};

#endif
