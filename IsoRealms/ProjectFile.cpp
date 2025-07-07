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

#include "Project.h"

namespace IsoRealms {
  ProjectFile::ProjectFile(Project& project) :
              cFile(project) {
  }

  ProjectFile::ProjectFile(Project& project, const std::string& filename, bool user) :
              ProjectFile(project) {
    cFile.setPath(filename, user);
  }

  std::vector<std::unique_ptr<IProperty>> ProjectFile::getProperties(Project& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<File>>("File", "TODO", cFile));
    for (const std::unique_ptr<ProjectFile>& mInclusion : cInclusions) {
      mProperties.emplace_back(std::make_unique<PropertyStruct>("Inclusion \"" + mInclusion->cFile.getRelativePath() + "\"", "TODO", "Edit...", [this, &mInclusion, &project]() {
        return mInclusion->getProperties(project);
      }, [this, &mInclusion]() {
        Utils::removeElementUnique(cInclusions, mInclusion.get());
      }));
    }
    mProperties.emplace_back(std::make_unique<PropertyAdd>("Inclusion", "TODO", "Add...", [this, &project]() {
      ProjectFile* mNewInclusion = cInclusions.emplace_back(std::make_unique<ProjectFile>(project)).get();
      return std::make_unique<PropertyStruct>("Inclusion \"" + mNewInclusion->cFile.getRelativePath() + "\"", "TODO", "Edit...", [this, &mNewInclusion, &project]() {
        return mNewInclusion->getProperties(project);
      }, [this, &mNewInclusion]() {
        Utils::removeElementUnique(cInclusions, mNewInclusion);
      });
    }));
    return mProperties;
  }
}

