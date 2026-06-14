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

#include <functional>
#include <map>
#include <string>
#include <ranges>
#include <algorithm>

#include "IsoRealms/Component.h"
#include "IsoRealms/IComponentTypeDefinition.h"
#include "IsoRealms/Persistence/JSONArray.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Project/ComponentType.h"
#include "IsoRealms/Project/ProjectFile.h"
#include "IsoRealms/ComponentLoader.h"
#include "IsoRealms/ComponentSaver.h"

// Forward declarations
namespace IsoRealms {
  class ProjectFile;
  class JSONArray;
  class JSONObject;
  class ComponentType;
}

namespace IsoRealms {
  template <typename MODULE, typename TYPE> class ComponentTypeDefinition : public IComponentTypeDefinition {
    private:
    class ComponentInfo {
      public:
      ComponentInfo(ComponentType& parent, MODULE& module, ProjectFile* ownerProject) :
                cComponentWrapper(parent, module, ownerProject) {
      }
      
      Component<MODULE, TYPE>* getComponentForUser(IComponentUser<TYPE>* user) {
        if (user != nullptr) {
          cUsers.emplace_back(user);
        }
        return &cComponentWrapper;
      }

      void release(IComponentUser<TYPE>* user) {
        Utils::removeElement(cUsers, user);
      }

      Component<MODULE, TYPE>* getComponent() {
        return &cComponentWrapper;
      }
      
      const Component<MODULE, TYPE>* getComponent() const {
        return &cComponentWrapper;
      }
      
      void notifyDeletion() {
        cComponentWrapper.removed();
        for (IComponentUser<TYPE>* user : cUsers) {
          user->relinquish(cComponentWrapper.getComponent());
        }
      }

      bool hasReadOnlyReferences() const {
        for (IComponentUser<TYPE>* user : cUsers) {
          if (user->isReadOnly()) {
            return true;
          }
        }
        return false;
      }

      void overrideReadOnlyReferences(ProjectFile* owner) {
        for (IComponentUser<TYPE>* user : cUsers) {
          if (user->isReadOnly()) {
            user->setOwner(owner);
          }
        }
      }

      private:
      Component<MODULE, TYPE> cComponentWrapper;
      std::vector<IComponentUser<TYPE>*> cUsers;
    };

    public: 
    class Iterator {
      public:
      Iterator() :
                cParent(nullptr),
                cIterator(cParent->cComponents.begin()) {
      }

      Iterator(const ComponentTypeDefinition<MODULE, TYPE>* const parent, typename std::map<std::string, std::unique_ptr<ComponentInfo>>::const_iterator iterator) :
                cParent(parent),
                cIterator(iterator) {
      }

      Iterator& operator++() {
        cIterator++;
        return *this;
      }
    
      bool operator!=(const Iterator& iterator) {
        return cParent != iterator.cParent || cIterator != iterator.cIterator;
      }
      
      TYPE* operator*() {
        return cIterator->second->getComponent()->getComponent();
      }
      
      private:
      const ComponentTypeDefinition<MODULE, TYPE>* cParent;
      typename std::map<std::string, std::unique_ptr<ComponentInfo>>::const_iterator cIterator;
    };

    ComponentTypeDefinition(MODULE& module) :
              cModule(module) {
    }
      
    void clear() {
      cComponents.clear();
    }

    IComponent* createComponent(ComponentType& parent, const std::string& name, ProjectFile* ownerProject) override {
      std::string mAvailableName = Utils::getAvailableKey(cComponents, name);
      IComponent* mComponent = cComponents.emplace(mAvailableName, std::make_unique<ComponentInfo>(parent, cModule, ownerProject)).first->second->getComponent();
      mComponent->publish();
      return mComponent;
    }
    
    IComponent* loadComponent(ComponentType& parent, const std::string& name, JSONObject object, ProjectFile* ownerProject) override {
      Component<MODULE, TYPE>* mComponent = cComponents.emplace(name, std::make_unique<ComponentInfo>(parent, cModule, ownerProject)).first->second->getComponent();
      ComponentLoader mLoader(mComponent->getComponentData(), object);
      mComponent->getComponent()->define(mLoader);
      mComponent->publish();
      return mComponent;
    }

    bool needsSaving(const ProjectFile* savingProject) const override {
      for (const std::unique_ptr<ComponentInfo>& mComponentInfo : cComponents | std::views::values) {
        if (mComponentInfo->getComponent()->needsSaving(savingProject)) {
          return true;
        }
      }
      return false;
    }
  
    void save(JSONObject& object, const ProjectFile* savingProject) override {
      for (const std::unique_ptr<ComponentInfo>& mComponentInfo : cComponents | std::views::values) {
        Component<MODULE, TYPE>* mComponent = mComponentInfo->getComponent();
        if (mComponent->needsSaving(savingProject)) {
          JSONObject mComponentObject = object.addObject(mComponent->getName());
          ComponentSaver mSaver(mComponent->getComponentData(), mComponentObject);
          mComponent->getComponent()->define(mSaver);
        }
      }
    }

    bool forEachComponent(std::function<bool(IComponent*)> func) override {
      for (const std::unique_ptr<ComponentInfo>& mComponentInfo : cComponents | std::views::values) {
        if (!func(mComponentInfo->getComponent())) {
          return false;
        }
      }
      return true;
    }

    IComponent* getComponent2(const std::string& name, bool required = true) const override {
      return getComponentInfo(name, required)->getComponent();
    }

    ComponentInfo* getComponentInfo(const std::string& name, bool required = true) const {
      typename std::map<std::string, std::unique_ptr<ComponentInfo>>::const_iterator mComponent = cComponents.find(name);
      if (mComponent == cComponents.end() && required) {
        std::cout << "Couldn't find component of name \"" + name + "\"  Available components" << std::endl;
        for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mPair : cComponents) {
          std::cout << "  " << mPair.first << std::endl;
        }
        std::cout << "End of components" << std::endl;
        throw ArgumentException("ERROR: ComponentTypeDefinition::getComponent: Component \"" + name + "\" not found.");
      }
      return mComponent == cComponents.end() ? nullptr : mComponent->second.get();
    }

    TYPE* getComponent(const std::string& name, bool required = true) const {
      return getComponentInfo(name, required)->getComponent()->getComponent();
    }

    TYPE* getComponentForClient(IComponentUser<TYPE>* user, const std::string& name, bool required = true) {
      return getComponentInfo(name, required)->getComponentForUser(user)->getComponent();
    }

    void release(IComponentUser<TYPE>* user, TYPE* component) {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent()->isComponent(component)) {
          mComponent.second->release(user);
          return;
        }
      }
    }
    
    std::vector<std::string> getAvailableComponents() const override {
      std::vector<std::string> mComponents;
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        mComponents.push_back(mComponent.first);
      }
      return mComponents;
    }

    const std::string& getComponentID(const IComponent& component) const override {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent() == &component) {
          return mComponent.first;
        }
      }
      throw ArgumentException("ERROR: ComponentTypeDefinition::getComponentID: Specified resource not known");
    }
    
    std::string getID(const TYPE* component) const {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent()->isComponent(component)) {
          return mComponent.first;
        }
      }
      throw ArgumentException("ERROR: ComponentTypeDefinition::getID: Specified resource not found.");
    }

    Iterator begin() const {
      return Iterator(this, cComponents.begin());
    }

    Iterator end() const {
      return Iterator(this, cComponents.end());
    }
    
    void deleteComponent(IComponent* component) override {
      for (typename std::map<std::string, std::unique_ptr<ComponentInfo>>::iterator it = cComponents.begin(); it != cComponents.end(); ++it) {
        if (it->second->getComponent() == component) {
          it->second->notifyDeletion();
          cComponents.erase(it);
          return;
        }
      }
    }

    void renameComponent(IComponent* component, const std::string& name) override {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent() == component) {
          if (mComponent.first != name) {
            cComponents.emplace(name, std::move(cComponents[mComponent.first]));
            cComponents.erase(mComponent.first);
            return;
          }
        }
      }
    }

    void refreshResourceRegistration(TYPE& resource) {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent()->isComponent(&resource)) {
          return mComponent.second->getComponent()->publish();
        }
      }
    }

    bool isReadOnly(const TYPE* component) const {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent()->isComponent(component)) {
          return mComponent.second->getComponent()->isReadOnly();
        }
      }
      return false;
    }

    void setOwner(const TYPE* component, ProjectFile* owner) {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent()->isComponent(component)) {
          mComponent.second->getComponent()->setOwner(owner);
          return;
        }
      }
    }

    bool hasReadOnlyReferences(const TYPE* component) const {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent()->isComponent(component)) {
          return mComponent.second->hasReadOnlyReferences();
        }
      }
      return false;
    }

    void overrideReadOnlyReferences(const TYPE* component) {
      for (const std::pair<const std::string, std::unique_ptr<ComponentInfo>>& mComponent : cComponents) {
        if (mComponent.second->getComponent()->isComponent(component)) {
          mComponent.second->overrideReadOnlyReferences(cModule.getProject().getProjectFile());
          return;
        }
      }
    }
    
    private:
    inline static const std::string JSON_ID = "id";

    MODULE& cModule;
    std::map<std::string, std::unique_ptr<ComponentInfo>> cComponents;
  };
}
