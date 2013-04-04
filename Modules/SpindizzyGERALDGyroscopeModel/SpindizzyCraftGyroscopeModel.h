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
#include <GL/glew.h>
#include <map>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/Texture/Texture.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class SpindizzyCraftGyroscopeModel:public I3DModel {
  private:
  Vertex* cLocation;

  static const float CIRCLE_RESOLUTION;
  static unsigned int cInstanceCount;
  static Texture* cTexture;

  void generateTextureDisc();
  void renderCircle(float, float, float, IColour*);
  void renderCircle(float, float, IColour*);

  public:
  SpindizzyCraftGyroscopeModel(Vertex*);

  /*********************\
   * Implements IModel *
  \*********************/
  void update(unsigned int);
  void render();

  ~SpindizzyCraftGyroscopeModel();
};

#endif
