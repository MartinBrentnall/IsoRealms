/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <memory>
#include <vector>

namespace IsoRealms {
  class JSONObject;
  class IAssetRegistry;
  class IAssets;
  class IModule;
  class IProject;
  class IProperty;
  class IResourceTypeRegistry;
  class LuaState;
  class ModuleOptions;

  class IModuleHandle {
    public:
    virtual void load(IProject& project, JSONObject object) = 0;
    virtual void save(JSONObject object) = 0;
    virtual void registerAssets(IAssetRegistry& assets) = 0;
    virtual std::vector<std::unique_ptr<IProperty>> getProperties() = 0;

    virtual ~IModuleHandle() {}
  };

#if __linux__
  typedef IModuleHandle* createModule(IProject* project, IResourceTypeRegistry* resourceTypeRegistry);
  typedef void destroyModule(IModuleHandle* module);
  typedef void initLuaInterfaces(LuaState* luaState);
#elif _WIN32
  typedef IModuleHandle* (__stdcall* createModule)(IProject* project, IResourceTypeRegistry* resourceTypeRegistry);
  typedef void (__stdcall* destroyModule)(IModuleHandle* module);
  typedef void (__stdcall* initLuaInterfaces)(LuaState* luaState);
#endif
}
