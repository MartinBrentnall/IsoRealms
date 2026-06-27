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

#include "IsoRealms/IComponentArraySource.h"
#include "IsoRealms/Resources/Client/ComponentOwner.h"
#include "IsoRealms/Types.h"

#include "Options.h"

namespace IsoRealms {
  class Project;

  class ProjectLaunchConfiguration : public IOwnedKeyedMember {
    public:
    ProjectLaunchConfiguration(Project& parent, ProjectFile& owner, const std::string& name = "");
    std::string getName() const override;
    ProjectFile* getOwnerProjectFile() const override;
    bool needsSaving(const ProjectFile* savingProject) const override;
    void define(IComponentDefiner& definer, Project& project);
    bool isOwnedBy(const ProjectFile& project) const;
    void getOptions(Options& options) const;

    private:
    class Option {
      public:
      Option(Project& parent, ProjectLaunchConfiguration& launch, const std::string& name = "");
      std::string getName() const;
      std::string getValue() const;
      void getProperties(IComponentDefiner& definer, ProjectLaunchConfiguration& launch, const Options& hint = Options::EMPTY);

      private:
      std::string cDefName;
      String cDefValue;
    };

    std::string cDefName;
    ComponentOwner cDefOwner;
    std::vector<std::unique_ptr<Option>> cDefOptions;

    bool isOptionNameUsed(const std::string& name, Option* option) const;
    std::string makeOptionName() const;
  };
}
