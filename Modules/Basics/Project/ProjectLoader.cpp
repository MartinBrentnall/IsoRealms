/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ProjectLoader.h"

#include <SFML/System.hpp>

namespace IsoRealms::Basics {
  ProjectLoader::ProjectLoader(std::function<void(bool)> endFunction) :
            cFile(""),
            cUser(true),
            cEndFunction(endFunction),
            cFirstUse(true),
            cDestructing(false),
            cDestructed(false) {
  }

  ProjectLoader::ProjectLoader(const std::string& file, bool user, std::function<void(bool)> endFunction) :
            cFile(file),
            cUser(user),
            cEndFunction(endFunction),
            cFirstUse(true),
            cDestructing(false),
            cDestructed(false) {
  }

  void ProjectLoader::newProject(Application& application) {
    try {
      std::unique_ptr<Project> mProject = std::make_unique<Project>(application, cEndFunction);
      cMutex.lock();
      cProject = std::move(mProject);
      cMutex.unlock();
    } catch (ParseException& parseException) {  // TODO: Should be a more appropriate exception here
      cError = parseException.getMessage();
      std::cout << "WARNING: ProjectLoader::newProject: Failed to load project due to exception: \"" << cError << "\"." << std::endl;
    }
  }

  void ProjectLoader::loadProject(Application& application) {
    try {
      std::unique_ptr<Project> mProject = std::make_unique<Project>(application, cEndFunction, cFile, cUser);
      cMutex.lock();
      cProject = std::move(mProject);
      cMutex.unlock();
    } catch (ParseException& parseException) {  // TODO: Should be a more appropriate exception here
      cMutex.lock();
      cError = parseException.getMessage();
      cMutex.unlock();
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
    if (cError != "") {
      throw ParseException(cError);
    }
    Project* mProject = cProject.get();
    cMutex.unlock();
    if (mProject != nullptr) {
      if (cFirstUse) {
        mProject->getApplication().initMainThread();
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

  bool ProjectLoader::matches(const std::string& file, bool user) {
    return file == cFile && user == cUser;
  }
}
