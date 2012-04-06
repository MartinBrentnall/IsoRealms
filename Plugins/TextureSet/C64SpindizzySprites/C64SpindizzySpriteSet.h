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
#include <IsoRealms/IPaletteListener.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/ITexture.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/PlugSocket.h>
#include <IsoRealms/Texture.h>

class C64SpindizzySpriteSet:public IPlugin,
                            public IPaletteListener {
  private:
  static const float CIRCLE_RESOLUTION;
  static const float CIRCLE_OUTLINE_OUTER;
  static const float CIRCLE_OUTLINE_INNER;
  static const float CIRCLE_COLOUR_OUTER;
  static const float CIRCLE_COLOUR_INNER;
  static const float SQUARE_OUTLINE_OUTER;
  static const float SQUARE_OUTLINE_INNER;
  static const float SQUARE_COLOUR_OUTER;
  static const float SQUARE_COLOUR_INNER;
  static const float SQUARE_TRIANGLE_OUTER;
  static const float SQUARE_TRIANGLE_INNER;
  static const float DIAMOND_OUTLINE_OUTER;
  static const float DIAMOND_OUTLINE_INNER;
  static const float DIAMOND_COLOUR_OUTER;
  static const float DIAMOND_COLOUR_INNER;
  static const float DIAMOND_SQUARE_OUTLINE_OUTER;
  static const float DIAMOND_SQUARE_OUTLINE_INNER;
  static const float DIAMOND_SQUARE_COLOUR_OUTER;
  static const float DIAMOND_SQUARE_COLOUR_INNER;

  
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

  std::map<std::string, Texture*> cTextures; 
  std::map<std::string, GLuint> cTextureIDs;

  IColour* cColour1;
  IColour* cColour2;
  IColour* cColour3;
  IColour* cOutlineColour;
  
  /*
   * The follow functions create template images that can be drawn on.
   */
  void clear(IColour*);
  void makeLiftSquareImage();
  void makeLiftSquareHalfImage();

  void renderCircle(float, IColour*);
  void renderCircle(float, float, IColour*);
  void renderSquare(float, float, IColour*);
  void renderSquareHalf(float, float, IColour*);
  void renderDiamond(float, float, IColour*);
  void renderDiamondHalf(float, float, IColour*);
  void renderDiamondEdges(float, float, IColour*);
  
  /**
   * Draw the plain square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftSquare();

  /**
   * Draw the non-symmetrical square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftSquareHalf();

  /**
   * Draw the full square lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftSquareBoth();

  /**
   * Draw the plain diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftDiamond();

  /**
   * Draw the non-symmetrical diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftDiamondHalf();

  /**
   * Draw the full diamond lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftDiamondBoth();

  /**
   * Draw the plain circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftCircle();

  /**
   * Draw the non-symmetrical circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftCircleHalf();

  /**
   * Draw the full circle lift texture to the specified image.
   *
   * @param Image&  The image to draw on.
   */
  void renderLiftCircleBoth();

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

