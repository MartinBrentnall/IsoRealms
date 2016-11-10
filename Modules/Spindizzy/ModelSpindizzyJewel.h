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
#ifndef MODEL_SPINDIZZY_JEWEL_H
#define MODEL_SPINDIZZY_JEWEL_H

#include <cmath>
#include <cstdlib>
#include <GL/glew.h>

#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class ModelSpindizzyJewel:public I3DModel {
  private:
  static unsigned int cReferenceCount;
  static GLuint cPanelDisplayList;
  static GLuint cFrameDisplayList;

  Vertex* cLocation;
  float cScale;
  float* cCycleSpeed;

  IColour** cColourFrame;
  std::vector<IColour*>* cColoursCycle;
  float cProgress;

  // TODO: Move these to some kind of generic library.
  /**
   * Render a triangle connected by the specified co-ordinates, and indented
   * inwards (towards the triangle's center) by the specified distance.
   *
   * @param Vertex  First Vertex of the triangle.
   * @param Vertex  Second Vertex of the triangle.
   * @param Vertex  Third Vertex of the triangle.
   * @param float       Distance of indentation.
   */
  static void renderInnerTriangle(Vertex& a, Vertex& b, Vertex& c, float lineWidth);

  /**
   * Render a triangle outline connected by the specified co-ordinates of the
   * specified width, inwards (towards the triangle's center) from the points.
   *
   * @param Vertex  First Vertex of the triangle.
   * @param Vertex  Second Vertex of the triangle.
   * @param Vertex  Third Vertex of the triangle.
   * @param float       Distance of indentation.
   */
  static void renderOuterTriangle(Vertex& a, Vertex& b, Vertex& c, float lineWidth);

  /**
   * TODO: Triangle class.
   * Given the specified triangle
   */
  static void renderCellTriangle(Vertex, Vertex, Vertex, Vertex&, Vertex&, Vertex&, float);

  static double getValue(Vertex a1, Vertex a2, Vertex b1, Vertex b2);
  static double distance(double a1, double a2, double b1, double b2);

  public:
  ModelSpindizzyJewel(Vertex*, float, std::vector<IColour*>*, IColour**, float*);
  
  void refresh();

  /***********************\
   * Implements I3DModel *
  \***********************/
  void update(unsigned int);
  void render();

  ~ModelSpindizzyJewel();
};

#endif
