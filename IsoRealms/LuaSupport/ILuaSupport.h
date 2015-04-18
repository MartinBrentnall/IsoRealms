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
#ifndef I_LUA_SUPPORT_H
#define I_LUA_SUPPORT_H

#include <string>
#include <vector>

#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/Sound/ISound.h>

#include "ILuaModule.h"

class IArgumentDefinition;
class IArgumentValue;
class IResourceRegistry;

class ILuaSupport {
  public:
  virtual void registerScript(const std::string&) = 0;
  virtual void setGlobalVariable(IArgumentDefinition*) = 0;
  virtual void executeScript(const std::string&, std::vector<IArgumentValue*>) = 0;
//   virtual IArgumentDefinition* createArgumentDefinition(DOMNodeWrapper*, IResourceAccessor*) = 0;
  virtual IArgumentValue* getArgumentValue(DOMNodeWrapper*, IResourceAccessor*) = 0;
  virtual void enableLuaSupport(InitLuaFunction*) = 0;
};

#endif