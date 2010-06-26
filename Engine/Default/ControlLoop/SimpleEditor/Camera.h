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
#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <cmath>
#include <string>
#include <map>

#include <IsoRealms/Vertex.h>

class Camera {
  private:
  class ViewPoint {
    public:
    Vertex cLocation;
    double cRotation;                       
    double cTilt;
    double cRoll;

    ViewPoint(Vertex location, float rotation, float tilt, float roll);
    void place();
    void rotate(float rotation, float tilt);
  };

  std::map<int, ViewPoint*> cViewPoints;
  ViewPoint* cCurrentViewPoint;
  ViewPoint* cPreviousViewPoint;
  float cProgress;

  float sine(float min, float max, float gradient);
  float getSineAngle(float oldAngle, float newAngle);

  public:
//   static const int NORMAL = 0;
//   static const int WIDE = 1;
//   static const int ISOMETRIC = 2;

  static const float TRANSITION_SPEED;

  bool input(SDL_Event&);


  Camera();
  ~Camera();
  void place();
  void addViewPoint(int id, Vertex location, float rotation, float tilt, float roll);
  void setViewPoint(int id);
  void updateCamera(int ticks);

  void move(float);
  void zoomTo(float zoom);
  void rotate(float, float);

  float getAngle();
  float getTilt();
};

#endif
