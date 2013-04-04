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
#ifndef SPINDIZZY_ENEMY_HANDLER_H
#define SPINDIZZY_ENEMY_HANDLER_H

#include <IsoRealms/DefaultElementHandler.h>

#include "SpindizzyEnemy.h"
#include "ISpindizzyEnemySet.h"

class SpindizzyEnemyHandler:public DefaultElementHandler<SpindizzyEnemy> {
  private:
  ISpindizzyEnemySet* cSpindizzyEnemySet;
  bool cReinit;
  float cVisibility;
//   IElementContainer* cElementContainer;
  std::vector<SpindizzyEnemy*> cEnemies;

  void resetEnemies();

  public:
  SpindizzyEnemyHandler(ISpindizzyEnemySet*);

  void elementAdded(SpindizzyEnemy* enemy);
  void render();
  void updateRuntime(int ticks);
};

#endif
