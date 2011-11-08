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
#ifndef FLAT_RECTANGLE_FACTORY_H
#define FLAT_RECTANGLE_FACTORY_H

#include <IsoRealms/PlugSocket.h>

#include "../../TextureSet/ITextureSet.h"

#include "../ISimpleModelFactory.h"

#include "FlatRectangle.h"

class FlatRectangleFactory:public ISimpleModelFactory {
  private:
  ITextureSet* cTextureSet;
  ITexture* cTexture;
  std::string cTextureName;
  float cSize;
  bool cFlip;

  void updateTexture();
  
  public:
  FlatRectangleFactory();

  /**********************************\
   * Implements ISimpleModelFactory *
  \**********************************/
  ISimpleModel* createModel(Vertex*, float);
  void destroyModel(ISimpleModel*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);
};

#endif
