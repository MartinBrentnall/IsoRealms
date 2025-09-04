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
#pragma once

#include <mutex>
#include <string>

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class ProjectLoader {
    public:
    ProjectLoader(std::function<void(bool)> endFunction);
    ProjectLoader(const std::string& file, bool user, std::function<void(bool)> endFunction);

    void newProject(Application& application);
    void loadProject(Application& application);
    bool isDestructReady();
    void setDestructing();
    void destruct();
    bool isDestructed();
    Project* getLoadedProject();
    std::string getError();
    bool matches(const std::string& file, bool user);
    
    private:
    std::unique_ptr<Project> cProject;
    std::string cError;
    std::string cFile;
    bool cUser;
    std::function<void(bool)> cEndFunction;
    bool cFirstUse;
    bool cDestructing;
    bool cDestructed;
    std::mutex cMutex;
    std::mutex cDestructMutex;
  };
}
