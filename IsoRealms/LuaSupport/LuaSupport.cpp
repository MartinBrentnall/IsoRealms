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
#include "LuaSupport.h"

const std::string LuaSupport::TYPE_BOOLEAN = "Boolean";
const std::string LuaSupport::TYPE_COLOUR = "Colour";
const std::string LuaSupport::TYPE_SOUND = "Sound";
const std::string LuaSupport::TYPE_INTEGER = "Integer";
const std::string LuaSupport::TYPE_FLOAT = "Float";
const std::string LuaSupport::TYPE_MODEL = "3DModel";
const std::string LuaSupport::TYPE_VERTEX = "Vertex";
const std::string LuaSupport::TYPE_SCRIPT_CALL = "ScriptCall";
const std::string LuaSupport::TYPE_PROJECT = "Project";
const std::string LuaSupport::TYPE_UNIVERSE = "Universe";

bool operator==(const ISound& a, const ISound& b) {
  return &a == &b; 
}
 
bool operator==(const IInteger& a, const IInteger& b) {
  return &a == &b; 
}
 
bool operator==(const IFloat& a, const IFloat& b) {
  return &a == &b; 
}
 
bool operator==(const IBoolean& a, const IBoolean& b) {
  return &a == &b; 
}
 
bool operator==(const IVertex& a, const IVertex& b) {
  return &a == &b; 
}
 
bool operator==(const IColour& a, const IColour& b) {
  return &a == &b; 
}
 
bool operator==(const IScriptCall& a, const IScriptCall& b) {
  return &a == &b;
}
 
bool operator==(const Project& a, const Project& b) {
  return &a == &b;
}
 
bool operator==(const IUniverse& a, const IUniverse& b) {
  return &a == &b;
}
 
LuaSupport::LuaSupport() {
  cLuaState = luaL_newstate();
  luabind::open(cLuaState);
  luaL_openlibs(cLuaState);
  
  luabind::module(cLuaState) [
    luabind::class_<ISound>(TYPE_SOUND.c_str())
       .def("play", &ISound::play) 
       .def(luabind::const_self == luabind::const_self)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IInteger>(TYPE_INTEGER.c_str())
       .def("setValue", &IInteger::setValue)
       .def("getValue", &IInteger::getValue)
       .def(luabind::const_self == luabind::const_self)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IFloat>(TYPE_FLOAT.c_str())
       .def("setValue", &IFloat::setValue)
       .def("getValue", &IFloat::getValue)
       .def(luabind::const_self == luabind::const_self)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IBoolean>(TYPE_BOOLEAN.c_str())
       .def("setValue", &IBoolean::setValue)
       .def("getValue", &IBoolean::getValue)
       .def(luabind::const_self == luabind::const_self)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<I3DModel>(TYPE_MODEL.c_str())
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IVertex>(TYPE_VERTEX.c_str())
       .def(luabind::const_self == luabind::const_self)
  ];

  luabind::module(cLuaState) [
    luabind::class_<IColour>(TYPE_COLOUR.c_str())
       .def(luabind::const_self == luabind::const_self)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IScriptCall>(TYPE_SCRIPT_CALL.c_str())
       .def("execute", &IScriptCall::execute)
       .def(luabind::const_self == luabind::const_self)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<Project>(TYPE_PROJECT.c_str())
       .def("finish", &Project::finish)
       .def(luabind::const_self == luabind::const_self)
  ];
  
  luabind::module(cLuaState) [
    luabind::class_<IUniverse>(TYPE_UNIVERSE.c_str())
       .def(luabind::const_self == luabind::const_self)
  ];
}

void LuaSupport::registerScript(const std::string& script) {
  luaL_dostring(cLuaState, script.c_str());
}

// IArgumentDefinition* LuaSupport::createArgumentDefinition(DOMNodeWrapper* node, IResourceAccessor* resources) {
//   std::string mType = node->getAttribute("type");
//   if (mType == TYPE_SOUND) {
//     return new ArgumentDefinitionReference(node, &TYPE_SOUND);
//   } else if (mType == TYPE_INTEGER) {
//     return new ArgumentDefinitionPrimitive(node, &TYPE_INTEGER);
//   } else if (mType == TYPE_FLOAT) {
//     return new ArgumentDefinitionPrimitive(node, &TYPE_FLOAT);
//   } else if (mType == TYPE_MODEL) {
//     return new ArgumentDefinitionReference(node, &TYPE_MODEL);
//   } else if (mType == TYPE_VERTEX) {
//     return new ArgumentDefinitionReference(node, &TYPE_VERTEX);
//   } else if (mType == TYPE_COLOUR) {
//     return new ArgumentDefinitionReference(node, &TYPE_COLOUR);
//   } else {
// //    return resources->getRegisteredArgumentDefinition(node);
//     return NULL;
//   }
// }

IArgumentValue* LuaSupport::getArgumentValue(DOMNodeWrapper* node, IResourceAccessor* resources) {
  std::string mType = node->getAttribute("type");
  std::string mValue = node->getAttribute("value");
  if (mValue == "" && mType != "Project") {
    return NULL;
  }
  if (mType == TYPE_SOUND) {
    ISound* mSound = resources->getSound(mValue);
    return new ArgumentValue<ISound>(mSound);
  }
  if (mType == TYPE_INTEGER) {
    IInteger* mInteger = resources->getInteger(mValue);
    return new ArgumentValue<IInteger>(mInteger);
  }
  if (mType == TYPE_BOOLEAN) {
    IBoolean* mBoolean = resources->getBoolean(mValue);
    return new ArgumentValue<IBoolean>(mBoolean);
  }
  if (mType == TYPE_FLOAT) {
    IFloat* mFloat = resources->getFloat(mValue);
    return new ArgumentValue<IFloat>(mFloat);
  }
  if (mType == TYPE_MODEL) {
    Vertex* mVertex = new Vertex(0, 0, 0);
    I3DModel* mModel = resources->getModel(mValue, mVertex);
    return new ArgumentValue<I3DModel>(mModel);
  }
  if (mType == TYPE_VERTEX) {
    IVertex* mVertex = resources->getVertex(mValue);
    return new ArgumentValue<IVertex>(mVertex);
  }
  if (mType == TYPE_COLOUR) {
    IColour* mColour = resources->getColour(mValue);
    return new ArgumentValue<IColour>(mColour);
  }
  return resources->getArgumentValueCustom(node);
}

void LuaSupport::executeScript(const std::string& name, std::vector<IArgumentValue*> argumentValues) {
  try {
    for (unsigned int i = 0; i < argumentValues.size(); i++) {
      std::string mArgFunction = name + "_arg" + Utils::toString(i);
//      std::cout << "  Setting argument " << i << "..." << std::endl;
      argumentValues[i]->setArgument(cLuaState, mArgFunction);
    }
//    std::cout << "  All arguments set, executing code..." << std::endl;
    luabind::call_function<void>(cLuaState, name.c_str());
  } catch (luabind::error error) {
    std::cout << "  Script error: " << error.what() << std::endl;
  }
}

void LuaSupport::setGlobalVariable(IArgumentDefinition* argument) {
  IArgumentValue* mArgument = argument->getDefaultValue();
  std::string mArgFunction = "globalVar_" + argument->getName() + "_arg0";
  mArgument->setArgument(cLuaState, mArgFunction);
}

void LuaSupport::enableLuaSupport(InitLuaFunction* initLuaFunction) {
  initLuaFunction(cLuaState);
}

