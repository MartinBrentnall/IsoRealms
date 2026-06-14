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
#include "IsoRealms/Editing/Property/IComponentDefiner.h"

namespace IsoRealms {
  class ProjectFile;

  template <typename OWNER, typename TYPE> class OwnedResource {
    public:
    OwnedResource(Project& project, ProjectFile& ownerProject, OWNER& owner) :
              cResource(owner),
              cOwner(project, &ownerProject),
              cInit(false) {
    }

    void init(JSONObject object, const std::string& tag, ProjectFile& owner) {
      if (object.hasMember(tag) && !cInit) {
        cResource.init(object, tag);
        cOwner.setProjectFile(&owner);
        cInit = true;
      }
    }

    TYPE* operator*() {
      return &cResource;
    }

    void save(JSONObject object, const std::string& tag, const ProjectFile& saving) const {
      if (cOwner.getProjectFile() == &saving) {
        cResource.save(object, tag);
      }
    }

    void getProperty(IComponentDefiner& definer, const Metadata& metadata, const std::string& name, ProjectFile* loadOwner = nullptr) {
      if (definer.loadsPersistedValues()) {
        if (loadOwner != nullptr && !cInit && definer.hasPersistedMember(name)) {
          definer.propertyResource(name, cResource);
          cOwner.setProjectFile(loadOwner);
          cInit = true;
        }
        return;
      }
      if (definer.savesPersistedValues()) {
        definer.propertyResource(name, cResource);
        return;
      }
      if (cOwner.isConfigurable()) {
        definer.scope(name, cResource.getTreeItemLabel(), [this, name](IComponentDefiner& definer) {
          definer.propertyResource("Value", cResource);
          cOwner.createProperty(definer, "Owner");
        });
      } else {
        definer.propertyResource(name, cResource);
      }
    }

    private:
    TYPE cResource;
    ComponentOwner cOwner;
    bool cInit; // TODO: I think this should be part of the local loading process, not a field here.
  };
}
