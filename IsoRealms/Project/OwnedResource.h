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

#include "IsoRealms/IComponentDefiner.h"

namespace IsoRealms {
  class ProjectFile;

  template <typename OWNER, typename TYPE> class OwnedResource {
    public:
    OwnedResource(Project& project, ProjectFile& ownerProject, OWNER& owner) :
              cResource(owner),
              cOwner(project, &ownerProject) {
    }

    TYPE* operator*() {
      return &cResource;
    }

    void define(IComponentDefiner& definer, const std::string& name, ProjectFile* loadOwner) {

      // Nest the resource inside a folder if it's configurable.
      if (cOwner.isConfigurable()) {
        definer.scope(name, cResource.getTreeItemLabel(), [this, name, &definer]() {
          definer.propertyResource(name, cResource, IComponentDefiner::HINT_OPTIONAL);
          cOwner.define(definer, "Owner");
        });
      } else {
        definer.propertyResource(name, cResource, IComponentDefiner::HINT_OPTIONAL);
      }

      // Set the owner project file when the resource is initialised.
      definer.onInitialised([this, loadOwner]() {
        cOwner.setProjectFile(loadOwner);
      });
    }

    private:
    TYPE cResource;
    ComponentOwner cOwner;
  };
}
