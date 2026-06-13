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

#include <string>

#include "IsoRealms/Metadata.h"
#include "IsoRealms/Editing/Property/IPropertyMaker.h"

namespace IsoRealms {
  class ProjectFile;

  template <typename OWNER, typename TYPE> class OwnedAsset {
    public:
    OwnedAsset(Project& project, ProjectFile& ownerProject, OWNER& owner) :
              cAsset(owner),
              cOwner(project, &ownerProject),
              cInit(false) {
    }

    void init(JSONObject object, const std::string& tag, ProjectFile& owner) {
      if (object.hasMember(tag) && !cInit) {
        cAsset.init(object, tag);
        cOwner.setProjectFile(&owner);
        cInit = true;
      }
    }

    TYPE* operator*() {
      return &cAsset;
    }

    void save(JSONObject object, const std::string& tag, const ProjectFile& saving) const {
      if (cOwner.getProjectFile() == &saving) {
        cAsset.save(object, tag);
      }
    }

    void getProperty(IPropertyMaker& owner, const Metadata& metadata, const std::string& name, ProjectFile* loadOwner = nullptr) {
      if (owner.loadsPersistedValues()) {
        if (loadOwner != nullptr && !cInit && owner.hasPersistedMember(name)) {
          owner.createPropertyTreeSelector(name, cAsset);
          cOwner.setProjectFile(loadOwner);
          cInit = true;
        }
        return;
      }
      if (owner.savesPersistedValues()) {
        owner.createPropertyTreeSelector(name, cAsset);
        return;
      }
      if (cOwner.isConfigurable()) {
        owner.createPropertyStruct(name, cAsset.getTreeItemLabel(), [this, &metadata, name](IPropertyMaker& owner) {
          owner.createPropertyTreeSelector("Value", cAsset);
          cOwner.createProperty(owner, "Owner");
        });
      } else {
        owner.createPropertyTreeSelector(name, cAsset);
      }
    }

    private:
    TYPE cAsset;
    ResourceOwner cOwner;
    bool cInit; // TODO: I think this should be part of the local loading process, not a field here.
  };
}
