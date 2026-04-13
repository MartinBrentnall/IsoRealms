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

  void ProjectFile::setDescription(JSONObject object) {
    cDefID = object.getString(JSON_DESCRIPTION);
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

  void ProjectFile::save(JSONObject object) const {
    object.addString(JSON_DESCRIPTION, cDefID);
    JSONArray mIncludeArray = object.addArray(JSON_INCLUDE);
    for (const std::unique_ptr<ProjectFile>& mInclusion : cInclusions) {
      JSONObject mIncludeObject = mIncludeArray.addObject();
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

  void ProjectFile::getProperties(IPropertyMaker& owner, const Metadata& metadata, Project& project, bool inclusion) {
    owner.createPropertyTreeSelector(metadata.getPropertyData("File"), cFile);
    owner.createPropertyNativeString(metadata.getPropertyData("Description"), [this]() {return cDefID;}, [this](const std::string& value) {cDefID = value;});
    if (inclusion && cFile.isUser()) {
      owner.createPropertyNativeBoolean(metadata.getPropertyData("AllowModifications"), [this]() {return cAllowModifications;}, [this, &owner, &project](bool value) {
        if (!value) {
          owner.confirm("Setting this file to read-only will cause it to be saved as it is currently.  Are you sure you want to do this?", [this, &project]() {
            project.save(*this);
            cAllowModifications = false;
          }, []() {
            // Nothing to do.
          });
        } else {
          cAllowModifications = true;
        }
      });
    }
    for (const std::unique_ptr<ProjectFile>& mInclusion : cInclusions) {
      owner.createPropertyStruct(metadata.getPropertyData("Inclusion"), mInclusion->cFile.getRelativePath(), [this, &mInclusion, &metadata, &project](IPropertyMaker& owner) {
        mInclusion->getProperties(owner, metadata, project, true);
      }, [this, &mInclusion]() {
        Utils::removeElementUnique(cInclusions, mInclusion.get());
      });
    }
    owner.createPropertyAdd(metadata.getPropertyData("Inclusion"), "Add...", [this, &owner, &metadata, &project]() {
      ProjectFile* mNewInclusion = cInclusions.emplace_back(std::make_unique<ProjectFile>(project)).get();
      owner.createPropertyStruct(metadata.getPropertyData("Inclusion"), mNewInclusion->cFile.getRelativePath(), [this, &mNewInclusion, &metadata, &project](IPropertyMaker& owner) {
        mNewInclusion->getProperties(owner, metadata, project, true);
      }, [this, &mNewInclusion]() {
        Utils::removeElementUnique(cInclusions, mNewInclusion);
      });
    });
  }

  const std::string ProjectFile::JSON_ALLOW_MODIFICATION = "allowModifications";
  const std::string ProjectFile::JSON_DESCRIPTION        = "description";
  const std::string ProjectFile::JSON_FILENAME           = "filename";
  const std::string ProjectFile::JSON_INCLUDE            = "include";
}

