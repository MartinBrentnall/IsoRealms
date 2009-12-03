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

#include "../../../Global/DummyPlugin.h"
#include "../../../Global/ICommandInfo.h"
#include "../../../Global/Image.h"
#include "../../../Global/IPlugin.h"
#include "../../../Global/PluginRegistry.h"
#include "../../../Global/PlugSocket.h"

#include "../../FourColourSupport/IFourColourSupport.h"
#include "../../FourColourSupport/IFourColourSupportListener.h"

#include "../ISpindizzyTextureSet.h"
#include "../ISpindizzyTexture.h"

#include "Commodore64SpindizzyTexture.h"

class Commodore64SpindizzyTextureSet:public ISpindizzyTextureSet,
                                     public IFourColourSupportListener {
  private:
  static const int RESOLUTION = 128;
  static const int GRID_WIDTH = RESOLUTION / 16;
  static const int EDGE_WIDTH = RESOLUTION / 8;

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

  std::map<TextureType, ISpindizzyTexture*> cTextures;

  IFourColourSupport* cColourScheme;

  // TODO: Make the texture set support returning a "transformed" texture (e.g. the returned texture also holds its own orientation to prevent duplication of rotated and flipped textures)
  Image* generatePlain();
  Image* generateSplitPlain();
  Image* generateIce(Colour* colour);
  Image* generateTrampoline();
  Image* generateSwitchSquare();
  Image* generateSwitchSquareHalf();
  Image* generateSwitchSquareBoth();
  Image* generateSwitchDiamond();
  Image* generateSwitchDiamondHalf();
  Image* generateSwitchDiamondBoth();
  Image* generateSwitchCircle();
  Image* generateSwitchCircleHalf();
  Image* generateSwitchCircleBoth();
  Image* generateArrow();
  Image* generateWallNorthSouth();
  Image* generateWallEastWest();
  Image* generateIceWall();

  /**
   * Utility method used for setting all pixels in the specified image to
   * full transparency.
   *
   * @param Image&  The image to draw on.
   */
  Image* makeTransparent();

  /**
   * Draw the plain square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftSquare();

  /**
   * Draw the non-symmetrical square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftSquareHalf();

  /**
   * Draw the full square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftSquareBoth();

  /**
   * Draw the plain diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftDiamond();

  /**
   * Draw the non-symmetrical diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftDiamondHalf();

  /**
   * Draw the full diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftDiamondBoth();

  /**
   * Draw the plain circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftCircle();

  /**
   * Draw the non-symmetrical circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftCircleHalf();

  /**
   * Draw the full circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  Image* generateLiftCircleBoth();

  GLuint registerTexture(Image*);

  void generateTextures();
  void destroyTextures();

  public:
  Commodore64SpindizzyTextureSet();

  /***********************************\
   * Implements ISpindizzyTextureSet *
  \***********************************/
  ISpindizzyTexture* getTexture(TextureType);

  /*****************************************\
   * Implements IFourColourSupportListener *
  \*****************************************/
  void fourColourPaletteChanged(IFourColourSupport*);

  /******************************************\
   * Implements IPluginSupport (in IPlugin) *
  \******************************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void notifyZoneAction(Zone*);
  void initPlugin(Zone*);
  std::vector<ICommandInfo*> getCommandInfo();
  void setEditingInfo(IComponentContainer*);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);

  ~Commodore64SpindizzyTextureSet();
};

#endif

