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

#include "../../Global/IElementSet.h"
#include "../../Global/IPluginSupport.h"
#include "../../Global/PluginRegistry.h"

#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/Collectables/ICollectables.h"
#include "../../Plugins/CommandRegistry/ICommandRegistry.h"
#include "../../Plugins/CommandRegistry/IUserCommand.h"

#include "ICollectablesAccessor.h"
#include "SpindizzyJewelFactory.h"

class SpindizzyJewelSet:public IElementSet,
                        public ICollectablesAccessor {
  private:
  std::vector<PlugSocket*> cJewelSockets;
  std::vector<IElementFactory*> cElementFactories;
  ISimpleModelFactory* cJewelModelFactory;
  ICollectables* cCollectables;

  ICommandRegistry* cCommandRegistry;
  std::vector<IUserCommand*> cJewelCollectedCommands;
  std::vector<IUserCommand*> cAllJewelsCollectedCommands;

  void setModel(ISimpleModelFactory*);

  public:
  SpindizzyJewelSet();

  /*************************\
   * ICollectablesAccessor *
  \*************************/
  ICollectables* getCollectables();
  void jewelCollected();
  void allJewelsCollected();

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
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
