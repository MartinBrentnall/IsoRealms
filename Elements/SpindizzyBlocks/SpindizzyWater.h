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
#ifndef SPINDIZZY_WATER_H
#define SPINDIZZY_WATER_H

#include <IsoRealms/Element.h>

#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProvider.h"
#include "../../Plugins/SurfaceProcessor/IWallSurface.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"

#include "ISpindizzyBlockFactory.h"
#include "ISurfaceProcessorProxy.h"
#include "TileSurface.h"
#include "WallSurface.h"

class SpindizzyWater:public Element<ISurfaceProcessorProxy, ISpindizzyBlockFactory>,
                     public ISurfaceProvider {
  private:
  // TODO: Need to define an "initialisation scheme" somewhere
  static const unsigned int INIT_REGISTER_BLOCKS;
  static const unsigned int INIT_PROCESS_BLOCKS;
  static const unsigned int INIT_REGISTER_SURFACES;
  static const unsigned int INIT_USE_SURFACES;

  ISpindizzyTextureSet** cSpindizzyTextureSet;
  BlockLocation cStartLocation;
  BlockLocation cEndLocation;
  
  /**
   * TODO: Document
   */
  std::vector<ITileSurfaceTemplate*> getWaterSurfaces();

  public:
  SpindizzyWater(ISpindizzyBlockFactory*, BlockLocation*, BlockLocation*, ISpindizzyTextureSet**);

  /*************************************************************************\
   * Implemented methods of IElement.h                                     *
  \*************************************************************************/
  void removed();
  void added();
  bool initElement(unsigned int);
  void renderStatic();
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, BlockLocation&);

  /*******************************\
   * Implements ISurfaceProvider *
  \*******************************/
  std::vector<ITileSurface*> getTileSurfaces(ITileSurface::FaceDirection);
  ISpindizzyTileSurface* createSubSurface(ITileSurface::FaceDirection, int, int, int, int);
  std::vector<IWallSurface*> getWallSurfaces(int, IWallSurface::FaceDirection);
  IWallSurface* createSubSurface(int, int, IWallSurface::FaceDirection, int, int, int, int, int);
  BlockArea* getCoverage();
  void setDirty();
};

#endif
