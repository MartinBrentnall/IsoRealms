/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef MODEL_TO_HUD
#define MODEL_TO_HUD

#include <IsoRealms/IsoRealmsConstants.h>

#include "../../3DModel/ISimpleModelFactory.h"
#include "../../Camera/ICamera.h"
#include "../../HUD/IHUD.h"

#include "../IUtilities.h"

class ModelToHUD:public IUtilities,
                 public IHUDComponentFactory,
                 public IHUDGameComponent {
  private:
  std::vector<PlugSocket*> cSockets;
  Vertex cModelLocation;
  ICamera* cCamera;
  ISimpleModelFactory* cModelFactory;
  ISimpleModel* cModel;
  IHUD* cHUD;

  public:
  ModelToHUD();
    
  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
  
  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent(const std::string&);

  /********************************\
   * Implements IHUDGameComponent *
  \********************************/
  void render(float, float);
  void update(int);
  float getAspectRatio();
};

#endif
