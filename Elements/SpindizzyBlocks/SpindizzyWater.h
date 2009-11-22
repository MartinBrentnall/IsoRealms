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

#include "../../Global/IElement.h"

#include "../../Plugins/RollableSurfaceCalculator/IRollableSurfaceCalculator.h"
#include "../../Plugins/RollableSurfaceCalculator/IRollableSurfaceProvider.h"
#include "../../Plugins/RollableSurfaceCalculator/IWallSurface.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"

#include "ISurfaceCalculator.h"
// TODO: Use a different class for water?
#include "RollableSurface.h"

class SpindizzyWater:public IElement,
                     public IRollableSurfaceProvider {
  private:
  enum InitStage {
    CACHE_SURFACES,
    CALCULATE_SURFACES
  };

  ISpindizzyTextureSet** cSpindizzyTextureSet;
  BlockLocation cStartLocation;
  InitStage cInitStage;
  int cEast;
  int cNorth;
  
  /**
   * TODO: Document
   */
  std::vector<IRollableSurface*> getWaterSurfaces();

  public:
  SpindizzyWater(IElementFactory*, BlockLocation*, int, int, ISpindizzyTextureSet**);

  /*************************************************************************\
   * Implemented methods of IElement.h                                     *
  \*************************************************************************/
  void removed();
  void added();
  bool initElement();
  void renderStatic();
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, BlockLocation&);

  /***************************************\
   * Implements IRollableSurfaceProvider *
  \***************************************/
  std::vector<IRollableSurface*> getRollableSurfaces(IRollableSurface::FaceDirection);
  IRollableSurface* createSubSurface(IRollableSurface::FaceDirection, int, int, int, int);
  std::vector<IWallSurface*> getWallSurfaces(int, IWallSurface::FaceDirection);
  IWallSurface* createSubSurface(int, int, IWallSurface::FaceDirection, int, int, int, int, int);
  BlockArea* getCoverage();
  void setDirty();
};

#endif
