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

#include <IsoRealms/Colour.h>
#include <IsoRealms/I3DModel.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Texture.h>
#include <IsoRealms/Vertex.h>

class SpindizzyCraftPyramidModel:public I3DModel {
  private:
  Vertex* cLocation;

  static unsigned int cInstanceCount;
  static const float OUTLINE;
  static Texture* cTextureTop;
  static Texture* cTextureSide;

  void generateTextureTop();
  void generateTextureSide();

  public:
  SpindizzyCraftPyramidModel(Vertex*);

  /***********************\
   * Implements I3DModel *
  \***********************/
  void update(int milliseconds);
  void render();

  ~SpindizzyCraftPyramidModel();
};

#endif
