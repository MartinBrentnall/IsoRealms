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
#ifndef LUA_SUPPORT_H
#define LUA_SUPPORT_H

extern "C" {
  #include "lua.h"
  #include "lualib.h"
}
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <string>
#include <vector>

#include <IsoRealms/Project.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Boolean/IBoolean.h>
#include <IsoRealms/Resources/Float/IFloat.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/Sound/ISound.h>
#include <IsoRealms/Utils.h>

#include "ArgumentValue.h"
#include "ILuaSupport.h"
#include "LuaGlobalVariable.h"
#include "LuaScript.h"

class LuaSupport:public ILuaSupport {
  private:
  static const std::string TYPE_BOOLEAN;
  static const std::string TYPE_COLOUR;
  static const std::string TYPE_SOUND;
  static const std::string TYPE_INTEGER;
  static const std::string TYPE_FLOAT;
  static const std::string TYPE_MODEL;
  static const std::string TYPE_VERTEX;
    
  lua_State* cLuaState;

  public:
  LuaSupport();
  void registerScript(const std::string&);
  void setGlobalVariable(IArgumentDefinition*);
  void executeScript(const std::string&, std::vector<IArgumentValue*>);
//   IArgumentDefinition* createArgumentDefinition(DOMNodeWrapper*, IResourceAccessor*);
  IArgumentValue* getArgumentValue(DOMNodeWrapper*, IResourceAccessor*);
  void enableLuaSupport(InitLuaFunction*);
};

#endif
