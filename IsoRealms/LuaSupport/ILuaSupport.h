#ifndef I_LUA_SUPPORT_H
#define I_LUA_SUPPORT_H

#include <string>
#include <vector>

#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/Sound/ISound.h>

#include "IArgumentSource.h"
#include "ILuaFunctionArgument.h"
#include "ILuaModule.h"

class IRuntimeContext;
class IZoneHandler;

class ILuaSupport {
  public:
  virtual void registerScript(const std::string&) = 0;
  virtual void setGlobalVariable(IArgumentDefinition*) = 0;
  virtual void executeScript(const std::string&, std::vector<ILuaFunctionArgument*>) = 0;
  virtual IArgumentDefinition* createArgumentDefinition(DOMNodeWrapper*, IResourceAccessor*) = 0;
  virtual IArgumentSource* createArgument(DOMNodeWrapper*, IResourceAccessor*) = 0;
  virtual void enableLuaSupport(InitLuaFunction*, IRuntimeContext*) = 0;
};

#endif