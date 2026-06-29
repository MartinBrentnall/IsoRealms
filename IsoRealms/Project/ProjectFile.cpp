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
#include "ProjectFile.h"

#include "IsoRealms/Editing/IComponentAccessManager.h"
#include "IsoRealms/Project/Options.h"
#include "IsoRealms/Utils.h"

#include "Project.h"

namespace IsoRealms {
  ProjectFile::ProjectFile(Project& project) :
            cFile(project) {
  }

  ProjectFile::ProjectFile(Project& project, const std::string& filename, bool user) :
              ProjectFile(project) {
    cFile.setPath(filename, user);
  }

  std::string ProjectFile::getName() const {
    return cDefID;
  }

  std::vector<std::string> ProjectFile::getNames() const {
    std::vector<std::string> mNames;
    getNames(mNames);
    return mNames;
  }

  void ProjectFile::getNames(std::vector<std::string>& names) const {
    if (cAllowModifications) {
      names.emplace_back(cDefID);
    }
    for (const std::unique_ptr<ProjectFile>& mInclusion : cInclusions) {
      mInclusion->getNames(names);
    }
  }

  ProjectFile* ProjectFile::getFile(const std::string& id) {
    if (id == cDefID) {
      return this;
    }
    for (const std::unique_ptr<ProjectFile>& mInclusion : cInclusions) {
      ProjectFile* mFile = mInclusion->getFile(id);
      if (mFile != nullptr) {
        return mFile;
      }
    }
    return nullptr; // TODO: Throw?
  }

  ProjectFile* ProjectFile::getFileByPath(const std::string& path) {
    if (cFile.isSet() && cFile.getRelativePath() == path) {
      return this;
    }
    for (const std::unique_ptr<ProjectFile>& mInclusion : cInclusions) {
      ProjectFile* mFile = mInclusion->getFileByPath(path);
      if (mFile != nullptr) {
        return mFile;
      }
    }
    return nullptr;
  }

  bool ProjectFile::isModifiable() const {
    return cAllowModifications && cFile.isUser();
  }

  void ProjectFile::rename(const std::string name, bool user) {
    cFile.setPath(name, user);
  }

  void ProjectFile::define(IComponentDefiner& definer, Project& project, bool inclusion, bool editing) {
    definer.propertyResource("filename", cFile, IComponentDefiner::HINT_TRANSIENT);
    definer.propertyString("description", cDefID);
    if (inclusion && cFile.isUser()) {
      definer.propertyBoolean("allowModification", cAllowModifications, true, nullptr, [this, &project](bool value, std::function<void()> confirm, std::function<void()> cancel, IComponentAccessManager& access) {
        if (!value) {
          access.confirm("Setting this file to read-only will cause it to be saved as it is currently.  Are you sure you want to do this?", [this, &project, confirm]() {
            project.save(*this);
            confirm();
          }, cancel);
        } else {
          confirm();
        }
      });
    }
    definer.array("include", cInclusions, [](const std::unique_ptr<ProjectFile>& inclusion) -> ProjectFile& {
      return *inclusion;
    }, [this, &project, &definer, editing](ProjectFile& inclusion) {
      if (editing) {
        definer.scope("include", inclusion.cFile.getRelativePath(), [&project, &inclusion, &definer, editing]() {
          inclusion.define(definer, project, true, editing);
        }, [this, &inclusion]() {
          Utils::removeElementUnique(cInclusions, &inclusion);
        });
      } else {
        definer.propertyResource("filename", inclusion.cFile, IComponentDefiner::HINT_IMMEDIATE);
        definer.scope("", "", [&project, &inclusion, &definer]() {
          project.define(definer, &inclusion);
        }, nullptr, IComponentDefiner::externalScopeHint(inclusion, inclusion.isModifiable()));
      }
    }, [this, &project]() -> ProjectFile& {
      return *cInclusions.emplace_back(std::make_unique<ProjectFile>(project)).get();
    });
  }
}
