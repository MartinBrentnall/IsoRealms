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
#ifndef C64_SPINDIZZY_TEXTURE_SET_H
#define C64_SPINDIZZY_TEXTURE_SET_H

#include <cmath>
#include <map>

#include <IsoRealms/Colour.h>
#include <IsoRealms/DummyPlugin.h>
#include <IsoRealms/IColour.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/IPaletteListener.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/ITexture.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/PlugSocket.h>
#include <IsoRealms/Texture.h>

#include "PlainColourTexture.h"

class C64SpindizzyTextureSet:public IPlugin,
                             public IPaletteListener {
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

  IRuntimeContext* cRuntimeContext;  
  std::map<std::string, Texture*> cTextures; 
  PlainColourTexture* cBackgroundTexture;
  std::map<std::string, GLuint> cTextureIDs;

  IColour* cFloorColour;
  IColour* cWallColour;
  IColour* cGridColour;
  IColour* cBackgroundColour;

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
  void renderSwitchSquare();
  void renderSwitchSquareHalf();
  void renderSwitchSquareBoth();
  void renderSwitchDiamond();
  void renderSwitchDiamondHalf();
  void renderSwitchDiamondBoth();
  void renderSwitchCircle();
  void renderSwitchCircleBoth();
  void renderSwitchCircleHalf();
  void renderArrow();
  void renderWallPlainMiddle();
  void renderWallMixedMiddle();
  void renderWallPlainCap();
  void renderWallMixedCap();
  void renderIceWall();

  void generateTextures();
  void destroyTextures();

  public:
  C64SpindizzyTextureSet(IRuntimeContext*);

  /*******************************\
   * Implements IPaletteListener *
  \*******************************/
  void paletteChanged(IPalette*, const std::string&);

  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);

  ~C64SpindizzyTextureSet();
};

#endif
