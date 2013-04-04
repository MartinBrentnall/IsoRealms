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
#ifndef SPINDIZZY_CRAFT_PYRAMID_MODEL_H
#define SPINDIZZY_CRAFT_PYRAMID_MODEL_H

#include <GL/glew.h>
#include <map>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/Texture/Texture.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class SpindizzyCraftPyramidModel:public I3DModel {
  private:
  Texture** cTextureTop;
  Texture** cTextureSide;
  Vertex* cLocation;

  public:
  SpindizzyCraftPyramidModel(Vertex*, Texture**, Texture**);

  /***********************\
   * Implements I3DModel *
  \***********************/
  void update(unsigned int);
  void render();
};

#endif
