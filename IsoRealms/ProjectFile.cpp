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
              cFile(project),
              cAllowModifications(true) {
  }

  ProjectFile::ProjectFile(Project& project, const std::string& filename, bool user) :
              ProjectFile(project) {
    cFile.setPath(filename, user);
  }

  ProjectFile::ProjectFile(Project& project, JSONObject object) :
              ProjectFile(project) {
    cFile.load(JSON_FILENAME, object);
    cAllowModifications = object.getBoolean(JSON_ALLOW_MODIFICATION, true);
  }

  void ProjectFile::save(JSONArray array) const {
    for (const std::unique_ptr<ProjectFile>& mInclusion : cInclusions) {
      JSONObject mIncludeObject = array.addObject();
      mInclusion->saveInclusion(mIncludeObject);
    }
  }

  void ProjectFile::saveInclusion(JSONObject object) const {
    cFile.save(JSON_FILENAME, object);
    object.addBoolean(JSON_ALLOW_MODIFICATION, cAllowModifications, true);
  }

  bool ProjectFile::isModifiable() const {
    return cAllowModifications && cFile.isUser();
  }

  void ProjectFile::rename(const std::string name, bool user) {
    cFile.setPath(name, user);
  }

  std::vector<std::unique_ptr<IProperty>> ProjectFile::getProperties(IPropertyOwner& owner, Project& project, bool inclusion) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<File>>(owner, PropertyData("File", "TODO"), cFile));
    if (inclusion && cFile.isUser()) {
      mProperties.emplace_back(owner.createPropertyNativeBoolean("AllowModifications", [this]() {return cAllowModifications;}, [this](bool value) {cAllowModifications = value;}));
    }
    for (const std::unique_ptr<ProjectFile>& mInclusion : cInclusions) {
      mProperties.emplace_back(std::make_unique<PropertyStruct>(owner, PropertyData("Inclusion", "TODO"), mInclusion->cFile.getRelativePath(), [this, &owner, &mInclusion, &project]() {
        return mInclusion->getProperties(owner, project, true);
      }, [this, &mInclusion]() {
        Utils::removeElementUnique(cInclusions, mInclusion.get());
      }));
    }
    mProperties.emplace_back(std::make_unique<PropertyAdd>(PropertyData("Inclusion", "TODO"), "Add...", [this, &owner, &project]() {
      ProjectFile* mNewInclusion = cInclusions.emplace_back(std::make_unique<ProjectFile>(project)).get();
      return std::make_unique<PropertyStruct>(owner, PropertyData("Inclusion", "TODO"), mNewInclusion->cFile.getRelativePath(), [this, &owner, &mNewInclusion, &project]() {
        return mNewInclusion->getProperties(owner, project, true);
      }, [this, &mNewInclusion]() {
        Utils::removeElementUnique(cInclusions, mNewInclusion);
      });
    }));
    return mProperties;
  }

  const std::string ProjectFile::JSON_ALLOW_MODIFICATION = "allowModifications";
  const std::string ProjectFile::JSON_FILENAME           = "filename";
}

