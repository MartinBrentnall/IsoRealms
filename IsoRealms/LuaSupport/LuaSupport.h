#ifndef LUA_SUPPORT_H
#define LUA_SUPPORT_H

extern "C" {
  #include "lua.h"
  #include "lualib.h"
}
#include <luabind/luabind.hpp>
#include <string>
#include <vector>

#include <IsoRealms/IZone.h>
#include <IsoRealms/Project.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Boolean/IBoolean.h>
#include <IsoRealms/Resources/Float/IFloat.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/Sound/ISound.h>
#include <IsoRealms/Resources/ZoneHandler/IZoneHandler.h>
#include <IsoRealms/Utils.h>

#include "ArgumentDefinition.h"
#include "ArgumentDefinitionPrimitive.h"
#include "ArgumentSource.h"
#include "IArgumentSource.h"
#include "ILuaSupport.h"

class LuaSupport:public ILuaSupport {
  private:
  static const std::string TYPE_BOOLEAN;
  static const std::string TYPE_SOUND;
  static const std::string TYPE_INTEGER;
  static const std::string TYPE_FLOAT;
  static const std::string TYPE_MODEL;
  static const std::string TYPE_ZONE_HANDLER;
  static const std::string TYPE_ZONE;
  static const std::string TYPE_PROJECT;
  static const std::string TYPE_VERTEX;
    
  lua_State* cLuaState;

  public:
  LuaSupport();
  void registerScript(const std::string&);
  void setGlobalVariable(IArgumentDefinition*);
  void executeScript(const std::string&, std::vector<ILuaFunctionArgument*>);
  IArgumentDefinition* createArgumentDefinition(DOMNodeWrapper*, IResourceAccessor*);
  IArgumentSource* createArgument(DOMNodeWrapper*, IResourceAccessor*);
  void enableLuaSupport(InitLuaFunction*, IRuntimeContext*);
};

#endif
