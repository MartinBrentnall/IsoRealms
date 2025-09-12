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
#include "IsoRealms/PropertyMaker.h"

namespace IsoRealms {
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

    void save(JSONObject object, const std::string& tag, ProjectFile& saving) {
      if (cOwner.getProjectFile() == &saving) {
        cAsset.save(object, tag);
      }
    }

    void getProperty(PropertyMaker& owner, const Metadata& metadata, const std::string& name) {
      owner.createPropertyStruct(metadata.getPropertyData(name), cAsset.getID(), [this, &metadata, name](PropertyMaker& owner) {
        owner.createPropertyAsset(metadata.getPropertyData("Value"), cAsset);
        owner.createPropertyAsset(metadata.getPropertyData("Owner"), cOwner);
      });
    }

    private:
    TYPE cAsset;
    ResourceOwner cOwner;
    bool cInit; // TODO: I think this should be part of the local loading process, not a field here.
  };
}
