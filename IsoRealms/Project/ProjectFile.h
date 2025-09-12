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

#include "IsoRealms/Assets/Client/File.h"
#include "IsoRealms/Persistence/JSONArray.h"

namespace IsoRealms {
  class JSONArray;
  class Metadata;
  class Project;

  class ProjectFile {
    public:
    ProjectFile(Project& project);
    ProjectFile(Project& project, const std::string& filename, bool user);
    ProjectFile(Project& project, JSONObject object);
    void setDescription(JSONObject object);
    std::string getName() const;
    std::vector<std::string> getNames() const;
    void getNames(std::vector<std::string>& names) const;
    ProjectFile* getFile(const std::string& id);
    void getProperties(PropertyMaker& owner, const Metadata& Metadata, Project& project, bool topFile);
    void rename(const std::string name, bool user);
    void save(JSONObject object) const;
    void saveInclusion(JSONObject object) const;
    bool isModifiable() const;

//    private:
    static const std::string JSON_ALLOW_MODIFICATION;
    static const std::string JSON_DESCRIPTION;
    static const std::string JSON_FILENAME;
    static const std::string JSON_INCLUDE;

    File cFile;
    std::string cDefID;
    bool cAllowModifications;
    std::vector<std::unique_ptr<ProjectFile>> cInclusions;
  };
}
