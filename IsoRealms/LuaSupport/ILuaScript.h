#ifndef I_LUA_SCRIPT_H
#define I_LUA_SCRIPT_H

#include <vector>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/IResources.h>

#include "IArgumentGenerator.h"
#include "ILuaFunctionArgument.h"

class IResources;

class ILuaScript:public IResource {
  public:
  virtual void execute(std::vector<ILuaFunctionArgument*>) = 0;
  virtual std::vector<ILuaFunctionArgument*> readArguments(DOMNodeWrapper*, IArgumentGenerator*, IArgumentGenerator*) = 0;
  
  // TODO: Seperate this to another interface.
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
  virtual void save(std::vector<ILuaFunctionArgument*>, DOMNodeWriter*, IResourceLocator*) = 0;
};

#endif
