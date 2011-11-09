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

#include <GL/glu.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/MiscFunctions.h>
#include <IsoRealms/ScreenConfiguration.h>

#include "../../LocationAwareness/ILocationAwareness.h"
#include "../../SequencePlayer/ISequencePlayer.h"

#include "../ICamera.h"

class SpindizzyCamera:public ICamera,
                      public IDynamicElement,
                      public IVisualElement,
                      public ISequence {
  private:
  
  class AbsoluteCommand:public IUserCommand {
    private:
    SpindizzyCamera* cParent;
    std::string cCommandName;
    float cDestination;
    
    public:
    AbsoluteCommand(SpindizzyCamera*, const std::string&, float);

    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getName();
  };
  
  class RelativeCommand:public IUserCommand {
    private:
    SpindizzyCamera* cParent;
    std::string cCommandName;
    float cAmount;
    
    public:
    RelativeCommand(SpindizzyCamera*, const std::string&, float);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getName();
  };
  
  class SetLocationCommand:public IUserCommand {
    private:
    SpindizzyCamera* cParent;
    std::string cModeName;
    int cTargetLocation;
    Vertex cOffset;
    
    public:
    SetLocationCommand(SpindizzyCamera*, const std::string&, int, float, float, float);
    
    void save(DOMNodeWriter*);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getName();
  };
    
  std::vector<IUserCommand*> cCameraCommands;
  std::vector<SetLocationCommand*> cModes;
  
  IRuntimeContext* cRuntimeContext;
  std::vector<ILocationAwareness*> cLocationAwareness;
  int cSelectedLocation;
  ISequencePlayer* cSequencePlayer;
  Vertex* cLocation;
  Vertex cOffset;
  float cTargetAngle;
  float cPreviousAngle;
  float cProgress;

  float getCurrentAngle();
  void changeAngle(float);
  void changeAngleRelative(float);
  
  float cSequencePosition;
  
  int cMinX;
  int cMaxX;
  int cMinY;
  int cMaxY;
  int cMinZ;
  int cMaxZ;

  public:
  SpindizzyCamera(IRuntimeContext*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  std::vector<IVisualElement*> getPreLoopRenderers();
  std::vector<IDynamicElement*> getPreLoopCommands();
  void initPlugin(IZone*, unsigned int);
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
  
  /*****************************\
   * Implements IVisualElement *
  \*****************************/
  void render();
  
  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(int);

  /************************\
   * Implements ISequence *
  \************************/
  void update(float);
  
  /**********************\
   * Implements ICamera *
  \**********************/
  float getAngle();
  float getTilt();
};

#endif
