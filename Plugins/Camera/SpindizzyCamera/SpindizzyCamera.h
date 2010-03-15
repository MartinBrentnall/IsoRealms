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
