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

#include <memory>
#include <string>
#include <vector>

namespace IsoRealms {
  class JSONObject;
  class IResourceData;
  class Project;
  class ProjectFile;
  class PropertyMaker;

  class IResource {
    public:
    virtual std::string getName() const = 0;
    virtual bool isReadOnly() const = 0;
    virtual bool needsSaving(const ProjectFile* savingProject) = 0;
    virtual void save(JSONObject object) = 0;
    virtual void registerAssets() = 0;
    virtual bool renderIcon() = 0;
    virtual void getProperties(PropertyMaker& propertyMaker) = 0;
    virtual IResourceData& getResourceData() = 0;
//    virtual void hintInUse(bool inUse) = 0;
    
    virtual ~IResource() {}
  };
}
