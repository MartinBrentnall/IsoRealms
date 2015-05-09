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
#ifndef ELEMENT_SPINDIZZY_WATER_H
#define ELEMENT_SPINDIZZY_WATER_H

#include <IsoRealms/Condition.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "IGeometricElement.h"
#include "ISpindizzyBlock.h"
#include "ISpindizzyBlockType.h"
#include "ISpindizzyBlockSet.h"
#include "IWallSurface.h"
#include "TileSurface.h"
#include "WallSurface.h"

class ElementSpindizzyWater:public ISpindizzyBlock,
                            public IGeometricElement,
                            public IElementBounds {
  private:
  // TODO: Need to define an "initialisation scheme" somewhere
  static const unsigned int INIT_PROCESS_BLOCKS;

  ISpindizzyBlockType* cWaterType;
  IElementContainer* cContainer;
  
  BlockLocation cStartLocation;
  BlockLocation cEndLocation;
  Condition* cCondition;
  ITexture** cTexture;
  
  std::vector<ISpindizzyTileSurface*> cStaticTileSurfaces;
  std::vector<ISpindizzyTileSurface*> cDynamicTileSurfaces;

  /**
   * TODO: Document
   */
  std::vector<ITileSurfaceTemplate*> getWaterSurfaces();

  public:
  ElementSpindizzyWater(ISpindizzyBlockType*, BlockLocation*, BlockLocation*, ITexture**, IElementContainer*);

  void createSampleSurfaces();
  
  /******************************\
   * Implements ISpindizzyBlock *
  \******************************/
  std::set<IBoolean*> getInputs();

  /***********************\
   * Implements IElement *
  \***********************/
  IElementType* getElementType();
  bool initElement(unsigned int);
  void renderStatic();
  void renderRuntime();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  IElementBounds* getBounds();
  PickedElement* pickElement(Vertex&, Vertex&);

  /******************\
   * IElementBounds *
  \******************/
  float getWest();
  float getEast();
  float getSouth();
  float getNorth();
  float getBottom();
  float getTop();
  
  /*******************************\
   * Implements ISurfaceProvider *
  \*******************************/
  std::vector<ITileSurface*> getTileSurfaces(ITileSurface::FaceDirection);
  ISpindizzyTileSurface* createSubSurface(ITileSurface::FaceDirection, int, int, int, int, Condition*);
  std::vector<IWallSurface*> getWallSurfaces(int, IWallSurface::FaceDirection);
  IWallSurface* createSubSurface(int, int, IWallSurface::FaceDirection, int, int, int, int, int);
  BlockArea* getCoverage();
  void destroyCoverage(BlockArea*);
  Condition* getCondition();
  bool isGhost();
  void setDirty();
  IElementContainer* getElementContainer();
};

#endif
