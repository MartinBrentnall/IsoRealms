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
#include "ResourceOwner.h"

#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/ProjectFile.h"

namespace IsoRealms {
  ResourceOwner::ResourceOwner(Project& project, ProjectFile* owner) :
            cProject(project),
            cOwner(owner) {
  }

  ProjectFile* ResourceOwner::getProjectFile() const {
    return cOwner;
  }

  void ResourceOwner::setProjectFile(ProjectFile* owner) {
    cOwner = owner;
  }

  bool ResourceOwner::isConfigurable() const {
    return cProject.getWritableProjectFileNames().size() > 1;
  }

  void ResourceOwner::createProperty(PropertyMaker& owner, const PropertyData& metadata) {
    if (cProject.getWritableProjectFileNames().size() > 1) {
      owner.createPropertyAsset(metadata, *this);
    }
  }

  std::string ResourceOwner::getID() const {
    return cOwner->getName();
  }

  bool ResourceOwner::renderAssetIcon() const {
    return false;
  }

  bool ResourceOwner::hasConfiguration() const {
    return false;
  }

  bool ResourceOwner::isDefaultConfigured() const {
    return true;
  }

  void ResourceOwner::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  Application& ResourceOwner::getApplication() const {
    return cProject.getApplication();
  }

  std::vector<std::string> ResourceOwner::getAvailableProviders() const {
    std::vector<std::string> mNames = cProject.getWritableProjectFileNames();
    std::string mThisName = cOwner->getName();
    bool mFound = false;
    for (std::string& mName : mNames) {
      if (mName == mThisName) {
        mFound = true;
        break;
      }
    }
    if (!mFound) {
      mNames.emplace_back(mThisName);
    }
    return mNames;
  }

  bool ResourceOwner::renderProviderIcon(const std::string& id) const {
    return false;
  }

  void ResourceOwner::setID(const std::string& id) {
    cOwner = cProject.getProjectFile(id);
  }
}
