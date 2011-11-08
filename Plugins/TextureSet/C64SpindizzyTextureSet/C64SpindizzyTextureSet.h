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

#include <IsoRealms/DummyPlugin.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/Image.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/PlugSocket.h>

#include "../../Palette/IPalette.h"
#include "../../Palette/IPaletteListener.h"

#include "../ITextureSet.h"
#include "../ITexture.h"

#include "C64SpindizzyTexture.h"
#include "PlainColourTexture.h"

class C64SpindizzyTextureSet:public ITextureSet,
                             public IPaletteListener {
  private:
  static const int RESOLUTION = 128;
  static const int GRID_WIDTH = RESOLUTION / 16;
  static const int EDGE_WIDTH = RESOLUTION / 8;

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

  std::map<std::string, C64SpindizzyTexture*> cTextures; 
  PlainColourTexture* cBackgroundTexture;
  std::map<std::string, GLuint> cTextureIDs;

  IPalette* cPalette;
  std::string cFloorColourName;
  std::string cWallColourName;
  std::string cGridColourName;
  std::string cBackgroundColourName;
  Colour* cFloorColour;
  Colour* cWallColour;
  Colour* cGridColour;
  Colour* cBackgroundColour;

  /*
   * The follow functions create template images that can be drawn on.
   */
  Image* makePlainImage(Colour*);
  Image* makeTileImage();
  Image* makeSwitchSquareImage();
  Image* makeSwitchDiamondImage();

  GLuint generatePlain();
  GLuint generateSplitPlain();
  GLuint generateIce();
  GLuint generateTrampoline();
  GLuint generateSwitchSquare();
  GLuint generateSwitchSquareHalf();
  GLuint generateSwitchSquareBoth();
  GLuint generateSwitchDiamond();
  GLuint generateSwitchDiamondHalf();
  GLuint generateSwitchDiamondBoth();
  GLuint generateSwitchCircle();
  GLuint generateSwitchCircleHalf();
  GLuint generateSwitchCircleBoth();
  GLuint generateArrow();
  GLuint generateWallPlainCap();
  GLuint generateWallPlainMiddle();
  GLuint generateWallMixedCap();
  GLuint generateWallMixedMiddle();
  GLuint generateIceWall();

  GLuint convertToTexture(Image*, const std::string&, bool = false);

  void generateTextures();
  void destroyTextures();
  
  void saveColour(DOMNodeWriter*, const std::string&, const std::string&);

  public:
  C64SpindizzyTextureSet();

  /***********************************\
   * Implements ISpindizzyTextureSet *
  \***********************************/
  ITexture* getTexture(const std::string&);

  /*******************************\
   * Implements IPaletteListener *
  \*******************************/
  void paletteChanged(IPalette*, const std::string&);

  /******************************************\
   * Implements IPluginSupport (in IPlugin) *
  \******************************************/
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);

  ~C64SpindizzyTextureSet();
};

#endif

