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
#ifndef ARGUMENT_VALUE_H
#define ARGUMENT_VALUE_H

#include <luabind/luabind.hpp>

#include "IArgumentValue.h"

template <class T> class ArgumentValue:public IArgumentValue {
  private:
  T* cValue;
  
  public:
  ArgumentValue(T* value) {
    cValue = value;
  }
  
  T* getValue() {
    return cValue;
  }

  void setArgument(lua_State* luaState, const std::string& argFunction) {
//    std::cout << "Setting argument: " << cValue << std::endl;
    luabind::call_function<void>(luaState, argFunction.c_str(), cValue);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
//     std::string mPath = "~" + cLocalResources->getPath(cValue);
//     node->addAttribute("value", mPath);
  }
  
  bool isDefaultArgument() {
    return false;
  }
};

#endif
