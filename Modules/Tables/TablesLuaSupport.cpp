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
#include "TablesLuaSupport.h"

#include <sol.hpp>

#include "Table/Table.h"

namespace IsoRealms {
  template <typename TYPE> void LuaBinding<TYPE>::bind(const std::string& bindFunction) const {
    cLuaState.getState()[bindFunction](cDefValue);
  }
  
  template class LuaBinding<Tables::Table>;

  namespace Tables {

#ifdef __linux__
    extern "C" void initLua(LuaState* luaState) {
#elif _WIN32
    extern "C" void __declspec(dllexport) __stdcall initLua(LuaState* luaState) {
#endif
      sol::state& mLua = luaState->getState();
      mLua.new_usertype<Table>("Table");
    }
  }
}
