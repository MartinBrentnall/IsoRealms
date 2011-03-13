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
#ifndef FLAT_RECTANGLE_FACTORY_H
#define FLAT_RECTANGLE_FACTORY_H

#include <IsoRealms/PlugSocket.h>

#include "../../SpindizzyTextureSetChanger/ISpindizzyTextureSetChanger.h"

#include "../ISimpleModelFactory.h"

#include "Block.h"

class BlockFactory:public ISimpleModelFactory,
                   public IChangeableTextureSet {
  private:
  std::vector<PlugSocket*> cTextureSetSocket;
  ISpindizzyTextureSet* cTextureSet;
  ISpindizzyTexture* cTexture;
  std::string cTextureName;

  void updateTexture();
  
  public:
  BlockFactory();

  /**********************************\
   * Implements ISimpleModelFactory *
  \**********************************/
  ISimpleModel* createModel(Vertex*, float);
  void destroyModel(ISimpleModel*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);

  /************************************\
   * Implements IChangeableTextureSet *
  \************************************/
  void setSpindizzyTextureSet(ISpindizzyTextureSet*);
};

#endif
