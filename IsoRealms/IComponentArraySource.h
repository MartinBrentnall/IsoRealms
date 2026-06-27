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

#include <cstdint>
#include <functional>
#include <iterator>
#include <string>

#include "IsoRealms/IComponent.h"

namespace IsoRealms {
  class Module;
  class ProjectFile;

  class IModuleKeyedArraySource {
    public:
    virtual ~IModuleKeyedArraySource() = default;

    virtual bool isEmpty() const = 0;
    virtual void forEachModule(const std::function<void(const std::string& key, Module& module)>& visit) = 0;
    virtual Module& defineNewMember(const std::string& key) = 0;
  };

  class IOwnedKeyedMember {
    public:
    virtual ~IOwnedKeyedMember() = default;

    virtual std::string getName() const = 0;
    virtual ProjectFile* getOwnerProjectFile() const = 0;
    virtual bool needsSaving(const ProjectFile* savingProject) const = 0;
  };

  class IOwnedKeyedArraySource {
    public:
    virtual ~IOwnedKeyedArraySource() = default;

    virtual bool isEmpty() const = 0;
    virtual void forEachMember(const std::function<void(const std::string& key, IOwnedKeyedMember& member)>& visit) = 0;
    virtual IOwnedKeyedMember& defineNewMember(const std::string& key) = 0;
  };

  class IComponentKeyedArraySource {
    public:
    virtual ~IComponentKeyedArraySource() = default;

    virtual bool isEmpty() const = 0;
    virtual void forEachComponent(const std::function<void(const std::string& key, IComponent& component)>& visit) = 0;
    virtual IComponent& defineNewMember(const std::string& key, ProjectFile* ownerProject) = 0;
  };

  class IKeyedArraySource {
    public:
    virtual ~IKeyedArraySource() = default;

    virtual bool isEmpty() const = 0;
    virtual void forEachMember(const std::function<void(const std::string& name, const std::function<void()>& define)>& visit) = 0;
    virtual void defineNewMember(const std::string& key) = 0;
  };

  class IArraySource {
    public:
    virtual ~IArraySource() = default;

    virtual bool isEmpty() const = 0;
    virtual void forEachMember(const std::function<void(const std::function<void()>& define)>& visit) = 0;
    virtual void defineNewMember() = 0;
  };

  class IFixedArraySource {
    public:
    virtual ~IFixedArraySource() = default;

    virtual unsigned int getCount() const = 0;
    virtual void forEachMember(const std::function<void(unsigned int index, const std::function<void()>& define)>& visit) = 0;
    virtual void defineAtIndex(unsigned int index) = 0;
  };

  template <typename CONTAINER, typename GET_MEMBER_FUNC, typename ADD_FUNC>
  class OwnedKeyedArraySource : public IOwnedKeyedArraySource {
    public:
    OwnedKeyedArraySource(const CONTAINER& container, GET_MEMBER_FUNC getMember, ADD_FUNC add) :
              cContainer(container),
              cGetMember(getMember),
              cAdd(add) {
    }

    bool isEmpty() const override {
      return std::begin(cContainer) == std::end(cContainer);
    }

    void forEachMember(const std::function<void(const std::string& key, IOwnedKeyedMember& member)>& visit) override {
      for (const auto& mElement : cContainer) {
        IOwnedKeyedMember& mMember = cGetMember(mElement);
        visit(mMember.getName(), mMember);
      }
    }

    IOwnedKeyedMember& defineNewMember(const std::string& key) override {
      return cAdd(key);
    }

    private:
    const CONTAINER& cContainer;
    GET_MEMBER_FUNC cGetMember;
    ADD_FUNC cAdd;
  };

  template <typename CONTAINER, typename GET_COMPONENT_FUNC, typename ADD_FUNC>
  class ComponentKeyedArraySource : public IComponentKeyedArraySource {
    public:
    ComponentKeyedArraySource(const CONTAINER& container, GET_COMPONENT_FUNC getComponent, ADD_FUNC add) :
              cContainer(container),
              cGetComponent(getComponent),
              cAdd(add) {
    }

    bool isEmpty() const override {
      return std::begin(cContainer) == std::end(cContainer);
    }

    void forEachComponent(const std::function<void(const std::string& key, IComponent& component)>& visit) override {
      for (const auto& mElement : cContainer) {
        IComponent& mComponent = cGetComponent(mElement);
        visit(mComponent.getName(), mComponent);
      }
    }

    IComponent& defineNewMember(const std::string& key, ProjectFile* ownerProject) override {
      return cAdd(key, ownerProject);
    }

    private:
    const CONTAINER& cContainer;
    GET_COMPONENT_FUNC cGetComponent;
    ADD_FUNC cAdd;
  };

  template <typename CONTAINER, typename GET_KEY_FUNC, typename GET_MODULE_FUNC, typename ADD_FUNC>
  class ModuleKeyedArraySource : public IModuleKeyedArraySource {
    public:
    ModuleKeyedArraySource(const CONTAINER& container, GET_KEY_FUNC getKey, GET_MODULE_FUNC getModule, ADD_FUNC add) :
              cContainer(container),
              cGetKey(getKey),
              cGetModule(getModule),
              cAdd(add) {
    }

    bool isEmpty() const override {
      return std::begin(cContainer) == std::end(cContainer);
    }

    void forEachModule(const std::function<void(const std::string& key, Module& module)>& visit) override {
      for (const auto& mElement : cContainer) {
        visit(cGetKey(mElement), cGetModule(mElement));
      }
    }

    Module& defineNewMember(const std::string& key) override {
      return cAdd(key);
    }

    private:
    const CONTAINER& cContainer;
    GET_KEY_FUNC cGetKey;
    GET_MODULE_FUNC cGetModule;
    ADD_FUNC cAdd;
  };

  template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename ADD_FUNC>
  class KeyedArraySource : public IKeyedArraySource {
    public:
    KeyedArraySource(const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, ADD_FUNC add) :
              cContainer(container),
              cValue(value),
              cCreateProperty(createProperty),
              cAdd(add) {
    }

    bool isEmpty() const override {
      return std::begin(cContainer) == std::end(cContainer);
    }

    void forEachMember(const std::function<void(const std::string& name, const std::function<void()>& define)>& visit) override {
      for (const auto& mElement : cContainer) {
        visit(cValue(mElement).getName(), [this, mElement = &mElement]() {
          cCreateProperty(cValue(*mElement));
        });
      }
    }

    void defineNewMember(const std::string& key) override {
      cCreateProperty(cAdd(key));
    }

    private:
    const CONTAINER& cContainer;
    VALUE_FUNC cValue;
    PROPERTY_FUNC cCreateProperty;
    ADD_FUNC cAdd;
  };

  template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename ADD_FUNC>
  class ArraySource : public IArraySource {
    public:
    ArraySource(const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, ADD_FUNC add) :
              cContainer(container),
              cValue(value),
              cCreateProperty(createProperty),
              cAdd(add) {
    }

    bool isEmpty() const override {
      return std::begin(cContainer) == std::end(cContainer);
    }

    void forEachMember(const std::function<void(const std::function<void()>& define)>& visit) override {
      for (const auto& mElement : cContainer) {
        visit([this, mElement = &mElement]() {
          cCreateProperty(cValue(*mElement));
        });
      }
    }

    void defineNewMember() override {
      cCreateProperty(cAdd());
    }

    private:
    const CONTAINER& cContainer;
    VALUE_FUNC cValue;
    PROPERTY_FUNC cCreateProperty;
    ADD_FUNC cAdd;
  };

  template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC>
  class FixedArraySource : public IFixedArraySource {
    public:
    FixedArraySource(const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty) :
              cContainer(container),
              cValue(value),
              cCreateProperty(createProperty) {
    }

    unsigned int getCount() const override {
      return static_cast<unsigned int>(std::distance(std::begin(cContainer), std::end(cContainer)));
    }

    void forEachMember(const std::function<void(unsigned int index, const std::function<void()>& define)>& visit) override {
      unsigned int mIndex = 0;
      for (const auto& mElement : cContainer) {
        const unsigned int mCurrentIndex = mIndex++;
        visit(mCurrentIndex, [this, mElement = &mElement, mCurrentIndex]() {
          cCreateProperty(cValue(*mElement), mCurrentIndex);
        });
      }
    }

    void defineAtIndex(unsigned int index) override {
      auto mElement = std::begin(cContainer);
      std::advance(mElement, index);
      cCreateProperty(cValue(*mElement), index);
    }

    private:
    const CONTAINER& cContainer;
    VALUE_FUNC cValue;
    PROPERTY_FUNC cCreateProperty;
  };
}
