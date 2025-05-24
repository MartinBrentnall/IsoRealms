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
#include "ProjectLoader.h"

#include <SFML/System.hpp>

namespace IsoRealms::Basics {
  ProjectLoader::ProjectLoader(Options options, std::function<void(bool)> endFunction) :
            cOptions(options) {
    cOptions     = options;
    cEndFunction = endFunction;
    cProject     = nullptr;
    cFirstUse    = true;
    cError       = "";
    cDestructing = false;
    cDestructed  = false;
  }

  void ProjectLoader::loadProject(IApplication& application) {
    try {
      std::unique_ptr<Project> mProject = std::make_unique<Project>(application, cOptions, cEndFunction);
      cMutex.lock();
      cProject = std::move(mProject);
      cMutex.unlock();
    } catch (ParseException& parseException) {  // TODO: Should be a more appropriate exception here
      cError = parseException.getMessage();
      std::cout << "WARNING: ProjectLoader::loadProject: Failed to load project due to exception: \"" << cError << "\"." << std::endl;
    }
  }

  bool ProjectLoader::isDestructReady() {
    if (cDestructing) {
      return false;
    }
    cMutex.lock();
    bool mDestructReady = cProject != nullptr;
    cMutex.unlock();
    return mDestructReady;
  }
  
  void ProjectLoader::setDestructing() {
    cDestructing = true;
  }

  void ProjectLoader::destruct() {
    cMutex.lock();
    cProject.reset();
    cMutex.unlock();
    cDestructMutex.lock();
    cDestructed = true;
    cDestructMutex.unlock();
  }

  bool ProjectLoader::isDestructed() {
    cDestructMutex.lock();
    bool mDestructed = cDestructed;
    cDestructMutex.unlock();
    return mDestructed;
  }

  Project* ProjectLoader::getLoadedProject() {
    cMutex.lock();
    Project* mProject = cProject.get();
    cMutex.unlock();
    if (mProject != nullptr) {
      if (cFirstUse) {
        mProject->initMainThread();
        cFirstUse = false;
      } else {
        mProject->reset();
      }
    }
    return mProject;  
  }

  std::string ProjectLoader::getError() {
    return cError;
  }

  bool ProjectLoader::matches(const Options& options) {
    return options == cOptions;
  }
}
