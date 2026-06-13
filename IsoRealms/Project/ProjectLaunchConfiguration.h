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

#include "IsoRealms/Metadata.h"
#include "IsoRealms/Assets/Client/ComponentOwner.h"
#include "IsoRealms/Types.h"

#include "Options.h"

namespace IsoRealms {
  class Project;

  class ProjectLaunchConfiguration {
    public:
    ProjectLaunchConfiguration(Project& parent, ProjectFile& owner);
    ProjectLaunchConfiguration(Project& parent, ProjectFile& owner, JSONThing thing);
    std::string getName() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata, Project& project);
    void save(JSONObject object, const ProjectFile& savingProject) const;
    bool isOwnedBy(const ProjectFile& project) const;
    void getOptions(Options& options) const;

    private:
    class Option {
      public:
      Option(Project& parent, ProjectLaunchConfiguration& launch);
      Option(Project& parent, JSONThing thing);
      std::string getName() const;
      std::string getValue() const;
      void getProperties(IPropertyMaker& owner, const Metadata& metadata, ProjectLaunchConfiguration& launch);
      void save(JSONObject object) const;

      private:
      std::string cDefName;
      String cDefValue;
    };

    inline static const std::string JSON_OPTIONS = "options";

    std::string cDefName;
    ComponentOwner cDefOwner;
    std::vector<std::unique_ptr<Option>> cDefOptions;

    bool isOptionNameUsed(const std::string& name, Option* option) const;
    std::string makeOptionName() const;
  };
}
