/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef SPINDIZZY_CAMERA_H
#define SPINDIZZY_CAMERA_H

#include <GL/glew.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/MiscFunctions.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/Vertex/IVertex.h>
#include <IsoRealms/ScreenConfiguration.h>
#include <IsoRealms/SingleResourceTypeModule.h>

class SpindizzyCamera:public ICamera {
  private:
  IVertex* cTargetLocation;
  float cTargetAngle;
  float cTargetTilt;
  float cTargetZoom;
  IVertex* cPreviousLocation;
  float cPreviousAngle;
  float cPreviousTilt;
  float cPreviousZoom;
  bool cAngleUpdated;
  bool cTiltUpdated;
  bool cZoomUpdated;
  bool cLocationUpdated;
  float cProgress;
  IScript* cTransitionCompleteScript;
  
  float getCurrentAngle();
  float getCurrentTilt();
  float getCurrentZoom();

  public:
  SpindizzyCamera();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);

  void changeLocation(IVertex*);
  void changeLocationFixed(float, float, float);
  void changeZoom(float);
  void changeTilt(float);
  void changeAngle(float);
  void changeAngleRelative(float);
  void startTransition();
  float getTheTilt();
  float getTheAngle();
  float getTheZoom();
  IVertex* getLocation();

  /**********************\
   * Implements ICamera *
  \**********************/
  float getAngle();
  float getTilt();
  void render();
  void update(int);
};

#endif
