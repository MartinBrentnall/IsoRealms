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
#ifndef C64_SPINDIZZY_SPRITE_SET_H
#define C64_SPINDIZZY_SPRITE_SET_H

#include <cmath>
#include <map>

#include <IsoRealms/Colour.h>
#include <IsoRealms/DummyPlugin.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/Image.h>
#include <IsoRealms/IPaletteListener.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/ITexture.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/PlugSocket.h>

#include "C64SpindizzySprite.h"

class C64SpindizzySpriteSet:public IPlugin,
                            public IPaletteListener {
  private:
  static const int RESOLUTION = 128;
  static const int GRID_WIDTH = RESOLUTION / 16;
  static const int EDGE_WIDTH = RESOLUTION / 8;

  static const std::string LIFT_CIRCLE_BOTH;
  static const std::string LIFT_CIRCLE_ONE;
  static const std::string LIFT_CIRCLE_NONE;
  static const std::string LIFT_SQUARE_BOTH;
  static const std::string LIFT_SQUARE_ONE;
  static const std::string LIFT_SQUARE_NONE;
  static const std::string LIFT_DIAMOND_BOTH;
  static const std::string LIFT_DIAMOND_ONE;
  static const std::string LIFT_DIAMOND_NONE;
  
  /** Fully transparent constant. */
  Colour* TRANSPARENT;
  
  IRuntimeContext* cRuntimeContext;

  std::map<std::string, C64SpindizzySprite*> cTextures; 
  std::map<std::string, GLuint> cTextureIDs;

  IColour* cColour1;
  IColour* cColour2;
  IColour* cColour3;
  IColour* cOutlineColour;
  
  /*
   * The follow functions create template images that can be drawn on.
   */
  Image* makeTransparent();
  Image* makePlainImage(IColour*);
  Image* makeTileImage();
  Image* makeLiftSquareImage();
  Image* makeLiftSquareHalfImage();
  Image* makeLiftCircleImage();

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

  GLuint convertToTexture(Image*, const std::string&);

  void generateTextures();
  void destroyTextures();

  public:
  C64SpindizzySpriteSet(IRuntimeContext*);

  /**************************\
   * Implements ITextureSet *
  \**************************/
  ITexture* getTexture(const std::string&);

  /*******************************\
   * Implements IPaletteListener *
  \*******************************/
  void paletteChanged(IPalette*, const std::string&);

  
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);

  ~C64SpindizzySpriteSet();
};

#endif

