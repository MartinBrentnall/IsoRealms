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
#include <vector>

namespace IsoRealms {
  class JSONObject;
  class IProperty;
  class IComponentTypeRegistry;
  class LuaState;
  class ModuleOptions;
  class Project;
  class ResourcePublisher;

  class IModuleHandle {
    public:
    virtual void publish(ResourcePublisher& publisher) = 0;
    virtual void updateInputs(unsigned int milliseconds) = 0;
    virtual void updateRuntime(unsigned int milliseconds) = 0;
    virtual void updateEditing(unsigned int milliseconds) = 0;
    virtual void reset() = 0;

    template <typename COMPONENT_TYPE_DEFINITION> void updateInputs2(COMPONENT_TYPE_DEFINITION& resources, int milliseconds) {
      for (auto* mComponent : resources) {
        mComponent->updateInputs(milliseconds);
      }
    }
    
    template <typename COMPONENT_TYPE_DEFINITION> void updateRuntime2(COMPONENT_TYPE_DEFINITION& resources, int milliseconds) {
      for (auto* mComponent : resources) {
        mComponent->updateRuntime(milliseconds);
      }
    }
    
    template <typename COMPONENT_TYPE_DEFINITION> void updateEditing2(COMPONENT_TYPE_DEFINITION& resources, int milliseconds) {
      for (auto* mComponent : resources) {
        mComponent->updateEditing(milliseconds);
      }
    }
    
    template <typename COMPONENT_TYPE_DEFINITION> void reset2(COMPONENT_TYPE_DEFINITION& resources) {
      for (auto* mComponent : resources) {
        mComponent->reset();
      }
    }
    
    virtual ~IModuleHandle() {}
  };

#if __linux__
  typedef IModuleHandle* createModule(Project* project, IComponentTypeRegistry* resourceTypeRegistry);
  typedef void destroyModule(IModuleHandle* module);
  typedef void initLuaInterfaces(LuaState* luaState);
#elif _WIN32
  typedef IModuleHandle* (__stdcall* createModule)(Project* project, IComponentTypeRegistry* resourceTypeRegistry);
  typedef void (__stdcall* destroyModule)(IModuleHandle* module);
  typedef void (__stdcall* initLuaInterfaces)(LuaState* luaState);
#endif
}
