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
#ifndef LUA_SCRIPT_CALL_H
#define LUA_SCRIPT_CALL_H

#include <vector>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResources.h>
#include <IsoRealms/Resources/Script/IArgumentValueRegistry.h>
#include <IsoRealms/Resources/Script/IScript.h>
#include <IsoRealms/Resources/Script/IScriptCall.h>

class LuaScriptCall:public IScriptCall {
  private:
  IScript* cScript;
  std::vector<IArgumentValue*> cArgumentValues;
  
  public:
  LuaScriptCall(IScript*, std::vector<IArgumentValue*>);
    
  /**************************\
   * Implements IScriptCall *
  \**************************/
  void execute();
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
