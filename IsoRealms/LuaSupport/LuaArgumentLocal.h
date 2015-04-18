/*
 * Copyright 2015 Martin Brentnall
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
#ifndef LUA_ARGUMENT_LOCAL_H
#define LUA_ARGUMENT_LOCAL_H

#include <luabind/luabind.hpp>

#include "ArgumentValue.h"
#include "IArgumentLocator.h"
#include "ILuaFunctionArgument.h"

template <class T> class LuaArgumentLocal:public ILuaFunctionArgument {
  private:
  ArgumentValue<T>* cValue;
  IArgumentLocator* cLocalResources;

  public:
  LuaArgumentLocal(ArgumentValue<T>* value, IArgumentLocator* localResources) {
    cLocalResources = localResources;
    cValue = value;
  }
  
  void setArgument(lua_State* luaState, const std::string& argFunction) {
    luabind::call_function<void>(luaState, argFunction.c_str(), cValue->getValue());
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    std::string mPath = "~" + cLocalResources->getPath(cValue);
    node->addAttribute("value", mPath);
  }
  
  bool isDefaultArgument() {
    return false;
  }
};

#endif
