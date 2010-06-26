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

#include <GL/gl.h>
#include <map>

#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Image.h>
#include <IsoRealms/Vertex.h>

#include "../ISimpleModel.h"

class SpindizzyCraftPyramidModel:public ISimpleModel {
  private:
  Vertex* cLocation;

  enum TextureID {
    TEXTURE_TOP,
    TEXTURE_SIDE
  };

  static unsigned int cInstanceCount;
  static std::map<TextureID, GLuint> cTextures;

  GLuint convertToTexture(Image*);
  GLuint generateTextureTop();
  GLuint generateTextureSide();

  public:
  SpindizzyCraftPyramidModel(Vertex*);

  /**************************************************************************\
   * Implemented methods of ILogicRequirements.h (in ISimpleModel.h)        *
  \**************************************************************************/
  void update(int milliseconds);
  void render();

  ~SpindizzyCraftPyramidModel();
};

#endif
