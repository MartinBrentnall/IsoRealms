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
#ifndef CRAFT_H
#define CRAFT_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <math.h>
#include <map>
#include <iostream>

#include "General2DDrawing.h"
#include "SpindizzyConstants.h"
#include "Image.h"
#include "Coordinate.h"
#include "Point.h"

using namespace std;

class Craft:public SpindizzyConstants, General2DDrawing {
  private:
  // Craft types
  static const int PYRAMID = 1;
  static const int BALL = 2;
  static const int CIRCLE = 3;

  static const float STOP_THRESHOLD = 0.0001f;

  static int cCraftTypes[3];
  static float cCraftChangeProgress;

  // Texture types
  static const int PYRAMID_TOP = 1;
  static const int PYRAMID_SIDE = 2;
  static const int BALL_SPRITE = 3;
  static const int CIRCLE_DISC = 4; 

  static map<int, GLuint> cTextures;
  static bool cStaticInitialised;

  Coordinate cLocation;
  Coordinate cMomentum;
  Coordinate cStartLocation; // Player craft starts on grid lines, so we can't use Point
  Point cRespawnPoint;
  float cOldAlpha;
  float cAlphaProgress;
  float cDestAlpha; // Ghosts the enemy craft when it moves outside it's home zone
  bool cGhost;
  int* cType;

  static void generatePyramidTop(Image& image);
  static void generatePyramidSide(Image& image);
  static void generateBallSprite(Image& image);
  static void generateCircleDisc(Image& image);
  static void generateTexture(int type);
  static void generateTextures();

  void renderAsPyramid(float alpha);
  void renderAsBall(float alpha, float rotation, float tilt);
  void renderAsCircle(float alpha);

  public:
  static void toggleCraftTypes();
  static void updateCraftTypes(int ticks);

  Craft();
  Craft(Point& location);
  Craft(Point& location, int type);
  Point& getGridLocation();
  Coordinate& getLocation();
  void setRespawnPoint(Point);
  void setHeight(float height);
  void setLocation(Point&);
  void setLocation(Coordinate&);
  void changeMomentum(Coordinate cRelativeMomentum);
  void moveTowards(Coordinate& destination, float ticks);
  void moveToCamera();
  void respawn();
  void resetToOrigin();
  void land();
  Coordinate requestLocation(float ticks, bool airborne, bool ice, Coordinate&);

  void updateLocation(Coordinate);

  /**
   * Calculate collision time with this craft for the specified number of ticks
   * for another craft moving between the specified start and end locations.
   *
   * @param Coordinate&  Start location.
   * @param Coordinate&  End location.
   * @param float        Ticks passed during movement.
   * @returns            Ticks during which the two crafts intersect.
   */
  float getCollisionTime(Coordinate&, Coordinate&, float);
  private:
  float getCollisionTime(Coordinate&, Coordinate&, Coordinate&, Coordinate&);
  float getCollisionTime(Coordinate&, Coordinate, Coordinate);
  public:
  /**
   * Gradually slow down the craft on normal solid ground.
   */
  void updateMomentum(float ticks);

  /**
   * Using the slope and current X,Y momentum of the craft, we calculate the
   * Z momentum.  This is useful when jumping from surfaces.
   */
  bool calculateZMomentum(float, float);
  void updateVisuals(int ticks);
  void setGhost(bool enabled);
  void stop();
  void render(float alpha, float rotation, float tilt);
  void write(FILE *w);
  void debug();

  /**
   * This function is to be called when the craft collides with a wall.  It
   * causes the craft to bounce off the wall, or stop if the craft is going
   * slow. 
   *
   * @param int Direction of the wall to bounce from.
   * @return true if the craft stops, otherwise false if it bounces.
   */
  bool bounceWall(int);
  void bounceTrampoline();
};

#endif
