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

#include <set>
#include <string>

#include "IsoRealms/IComponent.h"
#include "IsoRealms/IComponentDefiner.h"
#include "IsoRealms/IComponentTypeDefinition.h"

namespace IsoRealms {
  class Module;

  class ComponentType {
    public:
    ComponentType(IComponentTypeDefinition* resourceType, Module& parent);
    void define(IComponentDefiner& definer);
    void loadPersistedMember(const std::string& name, bool isNull, IComponentDefiner& definer, ProjectFile* ownerProject);
    void reloadComponent(const std::string& resourceName);
    bool needsSaving(const ProjectFile* savingProject) const;

    std::string const getPlural() const;
    std::string const getSingular() const;
    bool forEachComponent(std::function<bool(IComponent*)> resourceFunction);
    const std::set<std::string> getDeletedComponents();
    IComponent* createComponent();
    void renameComponent(IComponent* component, const std::string& name);
    void deleteComponent(IComponent* component);
    void createOverriddenComponent(IComponent* component);
    void publishModuleResources();
    std::string getPath();
    const std::string& getName(const IComponent& component) const;
    std::string getDataPath(bool user);
    std::string getComponentPath();
    ProjectFile* getProjectFile();
    void makeUserDataDirectory(const std::string& resourceName);
    void renameUserDataDirectory(const std::string& oldName, const std::string& newName);
    std::string getProjectPathPrefix(bool user);
    std::string getCategory();
    std::string getDescription() const;
    Project& getProject();

    private:
    class PlaceHolder {
      public:
      PlaceHolder(const std::string& id, ProjectFile* ownerProject);
      std::string getID() const;
      ProjectFile* getProjectFile() const;
      bool needsSaving(const ProjectFile& savingProject) const;

      private:
      std::string cID;
      ProjectFile* cOwnerProject;
    };

    Module& cParent;
    IComponentTypeDefinition* cComponentType;
    std::set<std::unique_ptr<PlaceHolder>> cOmittedComponents;
    std::vector<std::unique_ptr<PlaceHolder>> cOverriddenComponents;
    std::string cSingular;
    std::string cPlural;
    std::string cCategory = "None";
    std::string cDescription;
  };
}
