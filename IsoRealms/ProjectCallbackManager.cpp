/*
 * Copyright 2023 Martin Brentnall
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
#include "ProjectCallbackManager.h"

#include "IProject.h"

namespace IsoRealms {
  ProjectCallbackManager::ProjectCallbackManager(IProject& project) :
            cProject(project) {
  }

  void ProjectCallbackManager::reset(std::function<void()> resetter) {
    cCallbackHandles.emplace_back(cProject.reset(*this, resetter));
  }

  void ProjectCallbackManager::updateRuntime(std::function<void(unsigned int)> dynamic) {
    cCallbackHandles.emplace_back(cProject.updateRuntime(*this, dynamic));
  }

  void ProjectCallbackManager::updateEditing(std::function<void(unsigned int)> dynamic) {
    cCallbackHandles.emplace_back(cProject.updateEditing(*this, dynamic));
  }

  ProjectCallbackManager::~ProjectCallbackManager() {
    for (ICallbackHandle* mCallbackHandle : cCallbackHandles) {
      cProject.removeCallback(mCallbackHandle);
    }
  }
}

