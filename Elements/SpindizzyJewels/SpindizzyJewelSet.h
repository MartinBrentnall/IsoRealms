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
#ifndef SPINDIZZY_GERALD_SET_H
#define SPINDIZZY_GERALD_SET_H

#include <vector>

#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/IElementSet.h>
#include <IsoRealms/IPluginSupport.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/Script.h>

#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/Collectables/ICollectables.h"

#include "ISpindizzyJewelSet.h"
#include "SpindizzyJewelFactory.h"

class SpindizzyJewelSet:public ISpindizzyJewelSet {
  private:
  std::vector<PlugSocket*> cJewelSockets;
  std::vector<IElementFactory*> cElementFactories;
  ISimpleModelFactory* cJewelModelFactory;
  ICollectables* cCollectables;

  ICommandRegistry* cCommandRegistry;
  Script* cJewelCollectedScript;
  Script* cAllJewelsCollectedScript;

  void setModel(ISimpleModelFactory*);

  public:
  SpindizzyJewelSet();

  /**********************\
   * ISpindizzyJewelSet *
  \**********************/
  ICollectables* getCollectables();
  void jewelCollected();
  void allJewelsCollected();

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void setRuntimeContext(IRuntimeContext*);
  void destroy(IElement*);
  ~SpindizzyJewelSet();

  /**********************************************\
   * Implements IPluginSupport (in IElementSet) *
  \**********************************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);
};

#endif
