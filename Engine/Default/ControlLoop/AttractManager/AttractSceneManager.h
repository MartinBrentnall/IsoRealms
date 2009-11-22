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
#ifndef ATTRACT_SCENE_MANAGER_H
#define ATTRACT_SCENE_MANAGER_H

#include <iostream>
#include <vector>
#include <map>

#include "IAttract.h"

using namespace std;

class AttractSceneManager {
  private:
  int cFirstActiveLayer;
  int cLastActiveLayer;
  map<int, IAttract*> cActiveAttractServicesByLayer;

  public:

  /**
   * Default constructor.
   */
  AttractSceneManager();

  /**
   * Render all running scenes.
   */
  void render();

  /**
   * Update running scenes by the specified number of milliseconds.
   */
  void update(int);

  /**
   * Start the specified scene in the specified layer.
   *
   * @param IAttract*  The scene to start.
   * @param int        The layer to show the scene in.
   */
  void start(IAttract*, int);

  /**
   * Terminate the specified scene.
   *
   * @param IAttract*  The scene to terminate.
   */
  void terminate(IAttract*);

  /**
   * Return all completed scenes.
   * 
   * @returns  Completed scenes.
   */
  vector<IAttract*> getCompletedScenes();
};

#endif
