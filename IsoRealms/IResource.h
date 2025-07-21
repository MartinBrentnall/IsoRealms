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
  class IDialogManager;
  class IEditingContext;
  class IAssets;
  class IFont;
  class IProperty;
  class Project;
  class ProjectFile;
  class PropertyMaker;

  class IResource {
    public:
    virtual PropertyMaker getPropertyMaker() = 0;
    virtual std::vector<std::unique_ptr<IProperty>> getProperties(IDialogManager& dialogManager) = 0;
    virtual std::string getName() = 0;
    virtual bool renderIcon() = 0;
    virtual void hintInUse(bool inUse) = 0;
    virtual void save(JSONObject object) = 0;
    virtual void registerAssets() = 0;
    virtual void unregisterAssets(Project& project, IAssets& releaser) = 0;
    virtual std::string getResourceDataPath() const = 0;    
    virtual bool needsSaving(ProjectFile* savingProject) = 0;
    
    virtual ~IResource() {}
  };
}
