#ifndef LUA_GLOBAL_VARIABLE_H
#define LUA_GLOBAL_VARIABLE_H

#include <iostream>
#include <string>
#include <vector>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/IResources.h>

#include "ILuaScript.h"
#include "ILuaFunctionArgument.h"

class LuaGlobalVariable:public IResource {
  private:
  IArgumentDefinition* cArgument;

  public:
  void save(DOMNodeWriter*, IResourceLocator*);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
};

#endif
