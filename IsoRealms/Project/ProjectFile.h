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

#include <functional>
#include <queue>
#include <set>
#include <string>

#include "IsoRealms/Resources/Client/File.h"

namespace IsoRealms {
  class Project;

  class ProjectFile {
    public:
    ProjectFile(Project& project);
    ProjectFile(Project& project, const std::string& filename, bool user);
    std::string getName() const;
    std::vector<std::string> getNames() const;
    void getNames(std::vector<std::string>& names) const;
    ProjectFile* getFile(const std::string& id);
    void define(IComponentDefiner& definer, Project& project, bool topFile, bool editing);
    void rename(const std::string name, bool user);
    bool isModifiable() const;

//    private:
    File cFile;
    std::string cDefID;
    bool cAllowModifications = true;
    std::vector<std::unique_ptr<ProjectFile>> cInclusions;
  };
}
