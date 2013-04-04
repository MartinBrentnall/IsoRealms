#include "LuaSupport.h"

const std::string LuaSupport::TYPE_BOOLEAN = "Boolean";
const std::string LuaSupport::TYPE_SOUND = "Sound";
const std::string LuaSupport::TYPE_INTEGER = "Integer";
const std::string LuaSupport::TYPE_FLOAT = "Float";
const std::string LuaSupport::TYPE_MODEL = "3DModel";
const std::string LuaSupport::TYPE_ZONE_HANDLER = "ZoneHandler";
const std::string LuaSupport::TYPE_ZONE = "Zone";
const std::string LuaSupport::TYPE_PROJECT = "Project";
const std::string LuaSupport::TYPE_VERTEX = "Vertex";

LuaSupport::LuaSupport() {
  cLuaState = luaL_newstate();
  luabind::open(cLuaState);
  luaL_openlibs(cLuaState);
  
  luabind::module(cLuaState) [
    luabind::class_<ISound>(TYPE_SOUND.c_str())
       .def("play", &ISound::play) 
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IZone>(TYPE_ZONE.c_str())
       .def("getEast", &IZone::getZoneEast)
       .def("getWest", &IZone::getZoneWest)
       .def("getNorth", &IZone::getZoneNorth)
       .def("getSouth", &IZone::getZoneSouth)
       .def("getTop", &IZone::getZoneTop)
       .def("getBottom", &IZone::getZoneBottom)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IInteger>(TYPE_INTEGER.c_str())
       .def("setValue", &IInteger::setValue)
       .def("getValue", &IInteger::getValue)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IFloat>(TYPE_FLOAT.c_str())
       .def("setValue", &IFloat::setValue)
       .def("getValue", &IFloat::getValue)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IBoolean>(TYPE_BOOLEAN.c_str())
       .def("setValue", &IBoolean::setValue)
       .def("getValue", &IBoolean::getValue)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IZoneHandler>(TYPE_ZONE_HANDLER.c_str())
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<I3DModel>(TYPE_MODEL.c_str())
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IProject>(TYPE_PROJECT.c_str())
       .def("setZoneHandler", &IProject::setZoneHandler)
       .def("getEast", &IProject::getEast)
       .def("getWest", &IProject::getWest)
       .def("getNorth", &IProject::getNorth)
       .def("getSouth", &IProject::getSouth)
       .def("getTop", &IProject::getTop)
       .def("getBottom", &IProject::getBottom)
       .def("getAspectRatio", &IProject::getAspectRatio)
       .def("getZoneCount", &IProject::getZoneCount)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IVertex>(TYPE_VERTEX.c_str())
  ];
}

void LuaSupport::registerScript(const std::string& script) {
  luaL_dostring(cLuaState, script.c_str());
}

void LuaSupport::setGlobalVariable(IArgumentDefinition* argument) {
  ILuaFunctionArgument* mArgument = argument->createDefaultArgumentValue();
  std::string mArgFunction = "globalVar_" + argument->getName() + "_arg0";
  mArgument->setArgument(cLuaState, mArgFunction);
}

void LuaSupport::executeScript(const std::string& function, std::vector<ILuaFunctionArgument*> arguments) {
  for (unsigned int i = 0; i < arguments.size(); i++) {
    std::string mArgFunction = function + "_arg" + Utils::toString(i);
    arguments[i]->setArgument(cLuaState, mArgFunction);
  }
  luabind::call_function<void>(cLuaState, function.c_str());
}

IArgumentDefinition* LuaSupport::createArgumentDefinition(DOMNodeWrapper* node, IResourceAccessor* resources) {
  std::string mType = node->getAttribute("type");
  if (mType == TYPE_SOUND) {
    return new ArgumentDefinition<ISound>(node, resources, &TYPE_SOUND);
  } else if (mType == TYPE_INTEGER) {
    return new ArgumentDefinitionPrimitive<IInteger>(node, resources, &TYPE_INTEGER);
  } else if (mType == TYPE_FLOAT) {
    return new ArgumentDefinitionPrimitive<IFloat>(node, resources, &TYPE_FLOAT);
  } else if (mType == TYPE_MODEL) {
    return new ArgumentDefinition<I3DModel>(node, resources, &TYPE_MODEL);
  } else if (mType == TYPE_ZONE_HANDLER) {
    return new ArgumentDefinition<IZoneHandler>(node, resources, &TYPE_ZONE_HANDLER);
  } else if (mType == TYPE_ZONE) {
    return new ArgumentDefinition<IZone>(node, resources, &TYPE_ZONE);
  } else if (mType == TYPE_PROJECT) {
    return new ArgumentDefinition<IProject>(node, resources, &TYPE_PROJECT);
  } else if (mType == TYPE_VERTEX) {
    return new ArgumentDefinition<IVertex>(node, resources, &TYPE_VERTEX);
  } else {
    return resources->getRegisteredArgumentDefinition(node);
  }
}

IArgumentSource* LuaSupport::createArgument(DOMNodeWrapper* node, IResourceAccessor* resources) {
  std::string mValue = node->getAttribute("value");
  std::string mType = node->getAttribute("type");
  if (mValue == "" && mType != "Project") {
    return NULL;
  }
  if (mType == TYPE_SOUND) {
    ISound* mSound = resources->getSound(mValue);
    return new ArgumentSource<ISound>(mSound);
  }
  if (mType == TYPE_INTEGER) {
    IInteger* mInteger = resources->getInteger(mValue);
    return new ArgumentSource<IInteger>(mInteger);
  }
  if (mType == TYPE_FLOAT) {
    IFloat* mFloat = resources->getFloat(mValue);
    return new ArgumentSource<IFloat>(mFloat);
  }
  if (mType == TYPE_MODEL) {
    Vertex* mVertex = new Vertex(0, 0, 0);
    I3DModel* mModel = resources->getModel(mValue, mVertex);
    return new ArgumentSource<I3DModel>(mModel);
  }
  if (mType == TYPE_ZONE_HANDLER) {
    IZoneHandler* mZoneHandler = resources->getZoneHandler(mValue);
    return new ArgumentSource<IZoneHandler>(mZoneHandler);
  }
  if (mType == TYPE_VERTEX) {
    IVertex* mVertex = resources->getLocation(mValue);
    return new ArgumentSource<IVertex>(mVertex);
  }
  if (mType == TYPE_PROJECT) {
    IProject* mProject = resources->getProject();
    return new ArgumentSource<IProject>(mProject);
  }
  return resources->getRegisteredArgument(node);
}

void LuaSupport::enableLuaSupport(InitLuaFunction* initLuaFunction, IRuntimeContext* runtimeContext) {
  initLuaFunction(cLuaState, runtimeContext);
}

