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

#include "../../../Global/MiscFunctions.h"

#include "../../LocationAwareness/ILocationAwareness.h"

#include "../ICamera.h"

class SpindizzyCamera:public ICamera,
                      public IDynamicElement,
                      public IInteractiveElement {
  private:
  std::vector<PlugSocket*> cLocationAwarenessSocket;
  ILocationAwareness* cLocationAwareness;
  Vertex* cLocation;
  float cTargetAngle;
  float cPreviousAngle;
  float cProgress;

  float getCurrentAngle();
  void keyDown(SDLKey&);
  void changeAngle(float);

  public:
  SpindizzyCamera();

  /******************\
   * IPluginSupport *
  \******************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  std::vector<IDynamicElement*> getPreLoopCommands();
  std::vector<IInteractiveElement*> getInteractiveElements();

  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(int);

  /**********************************\
   * Implements IInteractiveElement *
  \**********************************/
  bool input(SDL_Event&);

  /**********************\
   * Implements ICamera *
  \**********************/
  float getAngle();
  float getTilt();
};

#endif
