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
#ifndef LUA_GLOBAL_VARIABLE_H
#define LUA_GLOBAL_VARIABLE_H

#include <iostream>
#include <string>
#include <vector>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/IResources.h>
#include <IsoRealms/Resources/Script/IGlobalVariable.h>
#include <IsoRealms/Resources/Script/IScript.h>
#include <IsoRealms/Resources/IDummyModule.h>

#include "LuaScript.h"

class LuaGlobalVariable:public IGlobalVariable {
  private:
  IArgumentDefinition* cArgument;

  public:
  LuaGlobalVariable(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
    
  void save(DOMNodeWriter*, IResourceLocator*);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  virtual ~LuaGlobalVariable() {}
};

#endif
