/*
 * Copyright 2015 Martin Brentnall
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
#ifndef LAYER_INPUT_PLAYER_H
#define LAYER_INPUT_PLAYER_H

#include <fstream>
#include <iostream>
#include <vector>

#include <IsoRealms/Input/DigitalInput.h>
#include <IsoRealms/Project.h>
#include <IsoRealms/Resources/Layer/ILayer.h>

#include "IModuleInputPersistence.h"

class LayerInputPlayer : public ILayer {
  private:
  class InputEvent {
    
    public:
    unsigned int cID;
    bool cState;
    unsigned int cTime;
  };
    
  IModuleInputPersistence* cModule;
  std::ifstream* cRecording;
  Project* cProject;
  std::vector<DigitalInput*> cInputs;
  unsigned int cElapsedTime;
  InputEvent cNextEvent;
  bool cFinished;
    
  void readNextEvent();
  
  public:
  LayerInputPlayer(IModuleInputPersistence*);
    
  /*********************\
   * Implements ILayer *
  \*********************/
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void renderRuntime();
  void renderEditing();
  bool input(SDL_Event&);
  bool inputEditor(SDL_Event&);
  ElementPickRay* getPickRay(float, float);
  void initRuntime();
  void initEditor();
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  void staticChanged();    
  void resourceSelected(IElementType*);
  void reset();
  void addObjectSelectionListener(IObjectSelectionListener*);
};

#endif
