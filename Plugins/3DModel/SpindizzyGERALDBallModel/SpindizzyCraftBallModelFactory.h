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
#ifndef SPINDIZZY_CRAFT_BALL_MODEL_FACTORY_H
#define SPINDIZZY_CRAFT_BALL_MODEL_FACTORY_H

#include "../../Camera/ICamera.h"

#include "../ISimpleModelFactory.h"
#include "../ISimpleModel.h"

#include "SpindizzyCraftBallModel.h"

class SpindizzyCraftBallModelFactory:public ISimpleModelFactory {
  private:
  std::vector<PlugSocket*> cCameraSocket;
  ICamera* cCamera;

  public:
  SpindizzyCraftBallModelFactory();

  /**********************************\
   * Implements ISimpleModelFactory *
  \**********************************/
  ISimpleModel* createModel(Vertex*);
  void destroyModel(ISimpleModel*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
};

#endif
