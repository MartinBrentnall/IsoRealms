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
#ifndef SPINDIZZY_ENEMY_SET_H
#define SPINDIZZY_ENEMY_SET_H

#include <vector>

#include "../../Global/IElementSet.h"
#include "../../Global/PluginRegistry.h"

#include "../../Plugins/3DModel/ISimpleModelFactory.h"

#include "SpindizzyEnemyFactory.h"

class SpindizzyEnemySet:public IElementSet {
  private:
  std::vector<IElementFactory*> cElementFactories;
  ISimpleModelFactory* cEnemyModelFactory;

  void setModel(ISimpleModelFactory*);

  public:
  SpindizzyEnemySet();

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void destroy(IElement*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
  void save(DOMNodeWriter*);
};

#endif
