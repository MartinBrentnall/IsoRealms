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
#ifndef ARGUMENT_VALUE_LOCAL_H
#define ARGUMENT_VALUE_LOCAL_H

#include <luabind/luabind.hpp>

#include <IsoRealms/Resources/Script/IArgumentLocator.h>

#include "IArgumentValue.h"

template <class T> class ArgumentValueLocal:public IArgumentValue {
  private:
  IArgumentLocator* cArgumentLocator;
  T* cValue;
  
  public:
  ArgumentValueLocal(IArgumentLocator* argumentLocator) {
    cValue = nullptr;
    cArgumentLocator = argumentLocator;
  }
  
  void setValue(T* value) {
    cValue = value;
  }
  
  T* getValue() {
    return cValue;
  }

  void setArgument(lua_State* luaState, const std::string& argFunction) {
//    std::cout << "VALUE IS " << cValue << std::endl;
    luabind::call_function<void>(luaState, argFunction.c_str(), cValue);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    std::string mPath = "~" + cArgumentLocator->getPath(this);
    node->addAttribute("value", mPath);
  }
  
  bool isDefaultArgument() {
    return false;
  }
};

#endif
