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
#include "AttractSceneManager.h"

AttractSceneManager::AttractSceneManager() {
  cFirstActiveLayer = 0;
  cLastActiveLayer = 0;
}

void AttractSceneManager::render() {
  for (int i = cFirstActiveLayer; i <= cLastActiveLayer; i++) {
    if (cActiveAttractServicesByLayer[i] != NULL) {
      cActiveAttractServicesByLayer[i]->render();
    }
  }
}

void AttractSceneManager::update(int ticks) {
  for (int i = cFirstActiveLayer; i <= cLastActiveLayer; i++) {
    if (cActiveAttractServicesByLayer[i] != NULL) {
      cActiveAttractServicesByLayer[i]->update(ticks);
    }
  }
}

void AttractSceneManager::start(IAttract* attractScene, int layer) {
  for (int i = cFirstActiveLayer; i <= cLastActiveLayer; i++) {
    if (cActiveAttractServicesByLayer[i] == attractScene) {
      return;
    }
  }

  cout << "Starting scene in layer: " << layer << endl;
  attractScene->init();
  cActiveAttractServicesByLayer[layer] = attractScene;
  if (layer < cFirstActiveLayer) {
    cFirstActiveLayer = layer;
  }
  if (layer > cLastActiveLayer) {
    cLastActiveLayer = layer;
  }
}

void AttractSceneManager::terminate(IAttract* attractScene) {
  // TODO: Erase the scene properly (deallocate!)
  for (int i = cFirstActiveLayer; i <= cLastActiveLayer; i++) {
    if (cActiveAttractServicesByLayer[i] == attractScene) {
      cActiveAttractServicesByLayer[i] = NULL;
    }
  }
}

vector<IAttract*> AttractSceneManager::getCompletedScenes() {
  vector<IAttract*> mCompletedScenes;
  for (int i = cFirstActiveLayer; i <= cLastActiveLayer; i++) {
    if (cActiveAttractServicesByLayer[i] != NULL) {
      if (cActiveAttractServicesByLayer[i]->hasFinished()) {
        mCompletedScenes.push_back(cActiveAttractServicesByLayer[i]);
        cActiveAttractServicesByLayer[i] = NULL;
      }
    }
  }
  return mCompletedScenes;
}

