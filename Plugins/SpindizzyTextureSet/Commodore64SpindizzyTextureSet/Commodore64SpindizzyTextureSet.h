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
#ifndef COMMODORE_64_SPINDIZZY_TEXTURE_SET_H
#define COMMODORE_64_SPINDIZZY_TEXTURE_SET_H

#include <cmath>
#include <map>

#include <IsoRealms/DummyPlugin.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/Image.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/PlugSocket.h>

#include "../../FourColourSupport/IFourColourSupport.h"
#include "../../FourColourSupport/IFourColourSupportListener.h"

#include "../ISpindizzyTextureSet.h"
#include "../ISpindizzyTexture.h"

#include "Commodore64SpindizzyTexture.h"
#include "PlainColourTexture.h"

class Commodore64SpindizzyTextureSet:public ISpindizzyTextureSet,
                                     public IFourColourSupportListener {
  private:
  static const int RESOLUTION = 128;
  static const int GRID_WIDTH = RESOLUTION / 16;
  static const int EDGE_WIDTH = RESOLUTION / 8;

  static const std::string LIFT_CIRCLE_BOTH_NAME;
  static const std::string LIFT_CIRCLE_LEFT_NAME;
  static const std::string LIFT_CIRCLE_RIGHT_NAME;
  static const std::string LIFT_CIRCLE_NONE_NAME;
  static const std::string LIFT_SQUARE_BOTH_NAME;
  static const std::string LIFT_SQUARE_LEFT_NAME;
  static const std::string LIFT_SQUARE_RIGHT_NAME;
  static const std::string LIFT_SQUARE_NONE_NAME;
  static const std::string LIFT_DIAMOND_BOTH_NAME;
  static const std::string LIFT_DIAMOND_LEFT_NAME;
  static const std::string LIFT_DIAMOND_RIGHT_NAME;
  static const std::string LIFT_DIAMOND_NONE_NAME;

  std::vector<PlugSocket*> cPlugSockets;

  // TODO: Start nasty hack
  /** Fully transparent constant. */
  Colour* TRANSPARENT;

  /** Purple constant. */
  Colour* PURPLE;

  /** Yellow constant. */
  Colour* YELLOW;

  /** Black constant. */
  Colour* BLACK;
  // TODO: End nasty hack

  std::map<TextureType, Commodore64SpindizzyTexture*> cTextures;
  std::map<std::string, Commodore64SpindizzyTexture*> cTexturesByName; 
  PlainColourTexture* cBackgroundTexture;
  std::map<TextureType, GLuint> cTextureIDs;

  IFourColourSupport* cColourScheme;

  /*
   * The follow functions create template images that can be drawn on.
   */
  Image* makeTransparent();
  Image* makePlainImage(Colour*);
  Image* makeTileImage();
  Image* makeSwitchSquareImage();
  Image* makeSwitchDiamondImage();
  Image* makeLiftSquareImage();
  Image* makeLiftSquareHalfImage();
  Image* makeLiftCircleImage();

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
  GLuint generateWallNorthSouth();
  GLuint generateWallEastWest();
  GLuint generateIceWall();

  /**
   * Draw the plain square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftSquare();

  /**
   * Draw the non-symmetrical square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftSquareHalf();

  /**
   * Draw the full square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftSquareBoth();

  /**
   * Draw the plain diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftDiamond();

  /**
   * Draw the non-symmetrical diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftDiamondHalf();

  /**
   * Draw the full diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftDiamondBoth();

  /**
   * Draw the plain circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftCircle();

  /**
   * Draw the non-symmetrical circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftCircleHalf();

  /**
   * Draw the full circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  GLuint generateLiftCircleBoth();

  GLuint convertToTexture(Image*, TextureType);

  void generateTextures();
  void destroyTextures();

  public:
  Commodore64SpindizzyTextureSet();

  /***********************************\
   * Implements ISpindizzyTextureSet *
  \***********************************/
  ISpindizzyTexture* getTexture(TextureType);
  ISpindizzyTexture* getTexture(const std::string&);

  /*****************************************\
   * Implements IFourColourSupportListener *
  \*****************************************/
  void fourColourPaletteChanged(IFourColourSupport*);

  /******************************************\
   * Implements IPluginSupport (in IPlugin) *
  \******************************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  ~Commodore64SpindizzyTextureSet();
};

#endif

