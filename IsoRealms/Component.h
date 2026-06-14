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
 * You should have received a copy of the GNU General Public License
 *
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>

#include "Resources/Client/ComponentOwner.h"
#include "Editing/Property/IComponentDefiner.h"
#include "Editing/Property/PropertyNativeString.h"
#include "IActionContext.h"
#include "IComponent.h"
#include "IComponentData.h"
#include "Project/ResourcePublisher.h"
#include "Project/ComponentType.h"
#include "Project/Registry/ResourceIDException.h"
#include "Utils.h"

namespace IsoRealms {
  template <typename TYPE> concept HasHasReadOnlyReferences = requires(TYPE type) {
    {type.hasReadOnlyReferences()} -> std::convertible_to<bool>;
  };  

  template <typename TYPE> concept HasOverrideReadOnlyReferences = requires(TYPE type) {
    {type.overrideReadOnlyReferences()} -> std::convertible_to<void>;
  };

  template <typename TYPE> concept HasHintInUse = requires(TYPE type, bool inUse) {
    {type.hintInUse(inUse)} -> std::same_as<void>;
  };

  template <typename TYPE> concept HasRenderIcon = requires(TYPE type) {
    {type.renderIcon()} -> std::convertible_to<bool>;
  };

  template <typename TYPE> concept HasRemoved = requires(TYPE type) {
    {type.removed()} -> std::same_as<void>;
  };

  template <typename TYPE> concept HasPublish = requires(TYPE type, ResourcePublisher& publisher) {
    {type.publish(publisher)} -> std::same_as<void>;
  };

  template <typename MODULE, typename TYPE> class Component : public IComponent,
                                                                 public IComponentData,
                                                                 public IActionContext {
    public:
    Component(ComponentType& parent, MODULE& module, ProjectFile* ownerProject) :
              cParent(parent),
              cOwnerProject(parent.getProject(), ownerProject),
              cComponentHandle(module, static_cast<IComponentData&>(*this)),
              cPublisher(*this) {
    }
    
    Component(ComponentType& parent, MODULE& module, ProjectFile* ownerProject, JSONObject object) :
              cParent(parent),
              cOwnerProject(parent.getProject(), ownerProject),
              cComponentHandle(module, static_cast<IComponentData&>(*this), object),
              cPublisher(*this) {
    }

    TYPE* getComponent() {
      return &cComponentHandle;
    }
    
    /************************\
     * Implements IComponent *
    \************************/
    const std::string& getName() const override {
      return cParent.getName(*this);
    }
    
    void define(IComponentDefiner& definer) override {
      definer.propertyString("ComponentName", [this]() {return cParent.getName(*this);}, [this](const std::string& value) {
        cParent.renameUserDataDirectory(cParent.getName(*this), value);
        cParent.renameComponent(this, value);
        overrideReadOnlyReferences();
        publish();
        cParent.publishModuleResources();
      }, "", [this](const std::string& value) {
        return cParent.forEachComponent([this, &value](IComponent* mComponent) {
          return mComponent->getName() != value || mComponent == this;
        });
      }, nullptr, [this, &definer](std::function<void()> confirm, std::function<void()> cancel) {
        if (hasReadOnlyReferences()) {
          definer.confirm("TODO: This component is referenced by read-only components.  Renaming it will promote any read-only components referencing this one and make them writable.", [this, confirm]() {
            confirm();
          }, [this, cancel]() {
            cancel();
          });
        } else {
          confirm();
        }
      });
      cOwnerProject.createProperty(definer, "ComponentOwner");
      cComponentHandle.define(definer);
    }
    
    void removed() {
      if constexpr (HasRemoved<TYPE>) {
        cComponentHandle.removed();
      }
    }

    bool renderIcon() override {
      glPushMatrix();
      bool mRendered = false;
      if constexpr (HasRenderIcon<TYPE>) {
        mRendered = cComponentHandle.renderIcon();
      }
      if (!mRendered) {
        Utils::renderIconLeaf();
      }
      glPopMatrix();
      if (!cOwnerProject.getProjectFile()->isModifiable()) {
        glPushMatrix();
        glTranslatef(0.5f, -0.5f, 0.0f);
        glScalef(0.5f, 0.5f, 1.0f);
        Utils::renderIconLock();
        glPopMatrix();
      }
      return true;
    }
    
    void hintInUse(bool inUse) {
      if constexpr (HasHintInUse<TYPE>) {
        cComponentHandle.hintInUse(inUse);
      }
    }

    bool isComponent(const TYPE* component) const {
      return component == &cComponentHandle;
    }
    
    void publish() override {
      if constexpr (HasPublish<TYPE>) {
        cComponentHandle.publish(cPublisher);
      }
    }

    bool needsSaving(const ProjectFile* savingProject) override {
      return savingProject == cOwnerProject.getProjectFile();
    }
    
    IComponentData& getComponentData() override {
      return *this;
    }

    bool hasReadOnlyReferences() const override {
      if constexpr (HasHasReadOnlyReferences<TYPE>) {
        if (cComponentHandle.hasReadOnlyReferences()) {
          return true;
        }
      }
      return cPublisher.hasReadOnlyReferences();
    }

    void overrideReadOnlyReferences() override {
      if constexpr (HasOverrideReadOnlyReferences<TYPE>) {
        cComponentHandle.overrideReadOnlyReferences();
      }
      cPublisher.overrideReadOnlyReferences();
    }

    ProjectFile* getProjectFile() const override {
      return cOwnerProject.getProjectFile();
    }

    /****************************\
     * Implements IComponentData *
    \****************************/
    std::string getComponentID() const override {
      return cParent.getPath() + "/" + cParent.getName(*this);
    }

    std::string getComponentName() const override {
      return cParent.getName(*this);
    }
    
    std::string getPath(const std::string& file, bool user) const override {
      std::string mRelativePath = cOwnerProject.getProjectFile()->cFile.getRelativePath();
      mRelativePath = mRelativePath.substr(0, mRelativePath.find_last_of('.'));
      return cParent.getProjectPathPrefix(user) + mRelativePath + "/" + cParent.getComponentPath() + "/" + cParent.getName(*this) + "/" + file;
    }

    void makeUserDataDirectory() override {
      cParent.makeUserDataDirectory(cParent.getName(*this));
    }

    bool isIncluded() const override {
      return cParent.getProjectFile() != cOwnerProject.getProjectFile();
    }

    bool isReadOnly() const override {
      return !cOwnerProject.getProjectFile()->isModifiable();
    }

    void setOwner(ProjectFile* owner) override {
      if (!cOwnerProject.getProjectFile()->isModifiable()) {
        cParent.createOverriddenComponent(this);
      }
      cOwnerProject.setProjectFile(owner);
    }

    Project& getProject() override {
      return cParent.getProject();
    }

    const Project& getProject() const override {
      return cParent.getProject();
    }

    Project& getResourceManager() override {
      return cParent.getProject();
    }

    IActionContext& getDummyActionContext() override {
      return *this;
    }

    const Metadata& getMetadata() const override {
      return cParent.getMetadata();
    }

    void republish() override {
      publish();
    }

    IEventBindings* getBindingRegistry() override {
      return nullptr;
    }
    
    private:
    
    // External interfaces.
    ComponentType& cParent;
    
    // Structures.
    ComponentOwner cOwnerProject;
    TYPE cComponentHandle;
    ResourcePublisher cPublisher;
  };
}
