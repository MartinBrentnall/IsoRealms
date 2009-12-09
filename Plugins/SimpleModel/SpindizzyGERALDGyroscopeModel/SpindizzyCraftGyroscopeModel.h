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
#ifndef SPINDIZZY_CRAFT_GYROSCOPE_MODEL_H
#define SPINDIZZY_CRAFT_GYROSCOPE_MODEL_H

#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <map>

#include "../../../Global/IsoRealmsConstants.h"
#include "../../../Global/Image.h"

#include "../ISimpleModel.h"

class SpindizzyCraftGyroscopeModel:public ISimpleModel {
  private:
  enum TextureID {
    TEXTURE_DISC
  };

  static unsigned int cInstanceCount;
  static std::map<TextureID, GLuint> cTextures;

  GLuint generateTextureDisc();

  public:
  SpindizzyCraftGyroscopeModel();

  /**************************************************************************\
   * Implemented methods of ILogicRequirements.h (in ISimpleModel.h)        *
  \**************************************************************************/
  void update(int milliseconds);
  void render();

  ~SpindizzyCraftGyroscopeModel();
};

#endif
