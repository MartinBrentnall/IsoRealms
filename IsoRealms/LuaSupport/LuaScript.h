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
#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <iostream>
#include <string>
#include <vector>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/Integer/Integer.h>
#include <IsoRealms/Resources/IResources.h>
#include <IsoRealms/Resources/Script/IArgumentDefinition.h>
#include <IsoRealms/Resources/Script/IArgumentValueRegistry.h>
#include <IsoRealms/Resources/Script/IScript.h>

#include "ArgumentDefinitionPrimitive.h"
#include "ArgumentDefinitionReference.h"
#include "ILuaSupport.h"
#include "LuaScriptCall.h"

class LuaScript:public IScript {
  private:
  static const std::string TYPE_BOOLEAN;
  static const std::string TYPE_COLOUR;
  static const std::string TYPE_SOUND;
  static const std::string TYPE_INTEGER;
  static const std::string TYPE_FLOAT;
  static const std::string TYPE_MODEL;
  static const std::string TYPE_VERTEX;

  std::string cName;
  std::vector<IArgumentDefinition*> cArguments;
  std::string cCode;

  unsigned int getArgumentIndex(const std::string& name);
  
  public:
  LuaScript(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*); // TODO: Probably shouldn't be here
    
  void setCode(const std::string&);
  std::string getCode();
  void registerScript();

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  Icon<IScript>* getResourceIcon(IResourceBrowser<IScript>*);
  
  /**********************\
   * Implements IScript *
  \**********************/
  void execute(std::vector<IArgumentValue*>);
  IScriptCall* createScriptCall(DOMNodeWrapper*, IArgumentValueRegistry*, IArgumentValueRegistry*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  void save(std::vector<IArgumentValue*>, DOMNodeWriter*, IResourceLocator*);

  static IArgumentDefinition* createArgumentDefinition(DOMNodeWrapper*, IResourceRegistry*);
};

#endif
