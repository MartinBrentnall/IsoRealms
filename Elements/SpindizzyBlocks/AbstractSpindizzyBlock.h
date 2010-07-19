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
#ifndef ABSTRACT_SPINDIZZY_BLOCK_H
#define ABSTRACT_SPINDIZZY_BLOCK_H

#include <cmath>

#include <IsoRealms/BlockArea.h>
#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Condition.h>
#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/Element.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/MiscFunctions.h>

#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"
#include "../../Plugins/SurfaceProcessor/ITileSurface.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProvider.h"

#include "ISpindizzyBlockSet.h"
#include "ISpindizzyBlockFactory.h"
#include "SpindizzyBlockProperties.h"
#include "TileSurface.h"
#include "TileSplitSurface.h"
#include "WallSurface.h"

/**
 * TODO: Refactor dynamic_casts into static_casts.
 */
class AbstractSpindizzyBlock:public Element<ISpindizzyBlockSet, ISpindizzyBlockFactory>,
                             public ISurfaceProvider,
                             public IVisualElement {
  private:
  enum SplitType {
    NORTH_SOUTH,
    EAST_WEST    
  };

  // TODO: Need to define an "initialisation scheme" somewhere
  static const unsigned int INIT_REGISTER_BLOCKS;
  static const unsigned int INIT_PROCESS_BLOCKS;
  static const unsigned int INIT_REGISTER_SURFACES;
  static const unsigned int INIT_USE_SURFACES;
  
  std::vector<ISpindizzyTileSurface*> cStaticTileSurfaces;
  std::vector<ISpindizzyTileSurface*> cDynamicTileSurfaces;

  BlockLocation cStartLocation;
  BlockLocation cEndLocation;
  int cNorthWestHeight;
  int cNorthEastHeight;
  int cSouthWestHeight;
  int cSouthEastHeight;
  SplitType cSplitType;

  /**
   * When a block is sloped, the bottom of the block can be stepped along with
   * the slope.  When the block is flat or split, this flag has no effect.
   */
  bool cSteppedBottom;
  Condition* cCondition;

  /**
   * Return the stepping of the slope along the X axis.  A negative value
   * indicates sloping towards the east; a positive value indicates sloping
   * towards the west.  A value of zero indicates no sloping along the X axis.
   *
   * @returns  Slope along the X axis.
   */ 
  int getXSlope();

  /**
   * Return the stepping of the slope along the Y axis.  A negative value
   * indicates sloping towards the north; a positive value indicates sloping
   * towards the south.  A value of zero indicates no sloping along the Y axis.
   *
   * @returns  Slope along the Y axis.
   */ 
  int getYSlope();

  /**
   * Return tile surfaces for constructing this block.
   * 
   * TODO
   */ 
  std::vector<ITileSurfaceTemplate*> calculateTileSurfaces(const ITileSurface::FaceDirection);

  std::vector<IWallSurface*> calculateWallSurfaces(const IWallSurface::FaceDirection);

  /**
   * Determine if the surface of this block is flat.
   * 
   * @returns true if the surface is flat, otherwise false.
   */
  bool isFlat();

  ISpindizzyTextureSet::TextureType getWallTexture(WallSurface::FaceDirection);

  /**
   * Returns the location of the outer face (either X or Y value, depending on
   * the face direction).
   * 
   * @param  IWallSurface::FaceDirection  The facing direction of the outer
   *           face to find.
   * @returns  For north or south facing walls, the Y location of the specified
   *           outer face.  For east or west facing walls, the X location of
   *           the specified outer face.
   */
  int getOuterWallFaceLocation(IWallSurface::FaceDirection facing);

  /**
   * Returns the step slope of the wall for the specified direction.  This is
   * possible because the slope will always be the same for any given direction
   * regardless of where the wall is actually located.
   * 
   * @param IWallSurface::FaceDirection  The facing direction.
   * @returns  The step slope of the wall.
   */
  int getWallSlope(IWallSurface::FaceDirection);

  /**
   * Returns the minimum wall elevation for the specified direction.  This is
   * possible because the elevation will always be the same for any given
   * direction, regardless of where the wall is actually located.
   * 
   * @param IWallSurface::FaceDirection  The facing direction.
   * @returns  The minimum elevation of the wall facing the specified
   *           direction.  The minimum elevatation is the height by which the
   *           wall is raised from the base height of the tile at it's lowest
   *           point.
   */
  int getMinimumWallElevation(IWallSurface::FaceDirection facing);

  /**
   * Return the height of the tile surface at the specified cell.
   * 
   * @param int  X cell location.
   * @param int  Y cell location.
   * @returns  The step height of the specified cell.
   * @throws  Something TODO: Fill this in, e.g.  IllegalArgumentException.
   */
  int getTileSurfaceHeight(int, int);

  protected:

  /**
   * The texture set is represented by a pointer to a pointer to the actual
   * texture set.  This allows us to quickly change the texture set externally.
   */
  ISpindizzyTextureSet** cSpindizzyTextureSet;

  /**
   * Determine if the surface of this block is split.
   * 
   * @returns true if the surface is split, otherwise false.
   */
  bool isSplit();  

  int getBottomHeight(int, int);

  public:
  AbstractSpindizzyBlock(ISpindizzyBlockFactory*, BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  AbstractSpindizzyBlock(ISpindizzyBlockFactory*, ISpindizzyTextureSet**, DOMNodeWrapper*);

  ISpindizzyTextureSet* getTextureSet();

  virtual ISpindizzyTextureSet::TextureType getTileSurfaceTexture() = 0;
  virtual ISpindizzyTextureSet::TextureType getNorthWallTexture() = 0;
  virtual ISpindizzyTextureSet::TextureType getEastWallTexture() = 0;
  virtual ISpindizzyTextureSet::TextureType getSouthWallTexture() = 0;
  virtual ISpindizzyTextureSet::TextureType getWestWallTexture() = 0;

  /**
   * Flag this block for caching with the surface processor at next init.
   * This will also implicitly signal the element as dirty.
   */
  void cacheSurfaces();

  /**
   * Create a new rectangular surface by using the specified section of this
   * surface.  Values are inclusive!
   * 
   * @param IRollableSurface::FaceDirection  The direction that the surface
   *           faces.
   * @param int  North surface location.
   * @param int  East surface location.
   * @param int  South surface location.
   * @param int  West surface location.
   * @returns  The sub surface.
   */
  ISpindizzyTileSurface* createSubSurface(ITileSurface::FaceDirection, int, int, int, int, Condition*);

  /**
   * Create a new wall face surface according to the specified parameters.
   * 
   * @param int  X tile location of the wall.
   * @param int  Y tile location of the wall.
   * @param IWallSurface::FaceDirection  The direction that the wall faces,
   *           away from the tile location.
   * @param int  Length of the wall.
   * @param int  Bottom height of the wall.
   * @param int  Top height of the wall.
   * @param int  Bottom slope step of the wall.
   * @param int  Top slope step of the wall.
   * @returns  The wall surface meeting the specification.
   * @throws Something  TODO: If a wall cannot be created to the specification.
   */
  IWallSurface* createSubSurface(int, int, IWallSurface::FaceDirection, int, int, int, int, int);

  /***********************\
   * Implements IElement *
  \***********************/
  void removed();
  void added();
  bool initElement(unsigned int);
  void renderStatic();
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, BlockLocation&);

  /*****************************\
   * Implements IVisualElement *
  \*****************************/
  void render();

  /*******************************\
   * Implements ISurfaceProvider *
  \*******************************/
  std::vector<ITileSurface*> getTileSurfaces(ITileSurface::FaceDirection);
  std::vector<IWallSurface*> getWallSurfaces(int, IWallSurface::FaceDirection);
  Condition* getCondition();
  BlockArea* getCoverage();
  bool isGhost();
  void setDirty();
};

#endif
