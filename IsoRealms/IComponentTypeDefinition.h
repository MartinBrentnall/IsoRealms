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
#include <functional>

namespace IsoRealms {
  class ComponentType;
  class File;
  class IComponent;
  class IResourceRegistry;
  class ModuleOptions;
  class Project;
  class ProjectFile;
  class JSONObject;
  class JSONArray;

  class IComponentTypeDefinition {
    public:
    virtual IComponent* createComponent(ComponentType& parent, const std::string& name, ProjectFile* ownerProject) = 0;
    virtual IComponent* loadComponent(ComponentType& parent, const std::string& name, JSONObject object, ProjectFile* ownerProject) = 0;
    virtual void deleteComponent(IComponent* component) = 0;
    virtual void renameComponent(IComponent* component, const std::string& name) = 0;
    virtual IComponent* getComponent2(const std::string& name, bool required = true) const = 0;
    virtual std::vector<std::string> getAvailableComponents() const = 0;
    virtual const std::string& getComponentID(const IComponent& component) const = 0;
    virtual bool needsSaving(const ProjectFile* savingProject) const = 0;
    virtual void save(JSONObject& object, const ProjectFile* savingProject) = 0;
    virtual bool forEachComponent(std::function<bool(IComponent*)> func) = 0;
  };
}
