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
#include "LuaScript.h"

const std::string LuaScript::TYPE_BOOLEAN = "Boolean";
const std::string LuaScript::TYPE_COLOUR = "Colour";
const std::string LuaScript::TYPE_SOUND = "Sound";
const std::string LuaScript::TYPE_INTEGER = "Integer";
const std::string LuaScript::TYPE_FLOAT = "Float";
const std::string LuaScript::TYPE_MODEL = "3DModel";
const std::string LuaScript::TYPE_VERTEX = "Vertex";

LuaScript::LuaScript(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cName = node->getAttribute("name");
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Argument") {
      IArgumentDefinition* mArgument = createArgumentDefinition(mNode, resourceRegistry);
      cArguments.push_back(mArgument);
    } else if (mValueAsString == "Code") {
      std::string mCode = mNode->getStringValue();
      setCode(mCode);
    }
  }
  registerScript();
}

void LuaScript::setCode(const std::string& code) {
  cCode = code;
}

std::string LuaScript::getCode() {
  return cCode;
}

void LuaScript::registerScript() {
  std::string mFunction;
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    IArgumentDefinition* mArgument = cArguments[i];
    mFunction += mArgument->getCode(cName, i);
  }
  mFunction += "function " + cName + "()\n" + cCode + "\n";
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    mFunction += cArguments[i]->getCleanup();
  }
  mFunction += "end\n";
  std::cout << "Code is:\n" << mFunction << std::endl;
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->registerScript(mFunction);
}

void LuaScript::execute(std::vector<IArgumentValue*> arguments) {
//  std::cout << "Executing script \"" << cName << "\"" << std::endl;
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->executeScript(cName, arguments);
}

IScriptCall* LuaScript::createScriptCall(DOMNodeWrapper* node, IArgumentValueRegistry* globalArgs, IArgumentValueRegistry* localArgs) {
  std::vector<IArgumentValue*> mArguments;
  std::cout << "Reading " << cArguments.size() << " arguments for script " << cName << "..." << std::endl;

  for (unsigned int i = 0; i < cArguments.size(); i++) {
    mArguments.push_back(nullptr);
  }
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Argument") {
      std::string mValue = mNode->getAttribute("value");
      std::string mArgumentName = mNode->getAttribute("name");
      unsigned int mArgumentIndex = getArgumentIndex(mArgumentName);
      IArgumentValueRegistry* mResources;
      if (mValue[0] == '~') {
        mResources = localArgs;
      } else {
        mResources = globalArgs;
      }
      std::cout << "  Found value for argument " << mArgumentIndex << std::endl;
      mArguments[mArgumentIndex] = mResources->getArgumentValue(mNode);
    }
  }
  
  // If any arguments weren't specified, use the defaults
  for (unsigned int i = 0; i < mArguments.size(); i++) {
    if (mArguments[i] == nullptr) {
      std::cout << "  No value for argument " << i << "... using default " << std::endl;
      mArguments[i] = cArguments[i]->getDefaultValue();
      // TODO: Throw if no default value?
    }
  }
  return new LuaScriptCall(this, mArguments);
}

void LuaScript::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("name", cName);
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    DOMNodeWriter* mArgumentBranch = node->addBranch("Argument");
    cArguments[i]->save(mArgumentBranch, resourceLocator);
  }
  DOMNodeWriter* mCodeBranch = node->addBranch("Code");
  mCodeBranch->addText(cCode);
}

void LuaScript::save(std::vector<IArgumentValue*> argumentValues, DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  std::string mScriptPath = resourceLocator->getPath(this);
  node->addAttribute("name", mScriptPath);
  for (unsigned int i = 0; i < argumentValues.size(); i++) {
    
    /*
     * TODO:  Notice that default argument is checked twice here in two
     * different ways.  The second one is the one that actually works.  The
     * first one always returns false.  I'm not entirely sure what the
     * intention was behind the first mechanism, but *maybe* the second one can
     * fail if the user chooses a custom argument that happens to be the same
     * as the default one.  So maybe the first one should be used and the
     * 'default' should be a flag in the argument rather than an equality
     * check like the second one.
     */
    if (!argumentValues[i]->isDefaultArgument()) {
      if (!cArguments[i]->isDefaultValue(argumentValues[i])) {
        DOMNodeWriter* mArgumentBranch = node->addBranch("Argument");
        cArguments[i]->saveCall(mArgumentBranch, resourceLocator);
        argumentValues[i]->save(mArgumentBranch, resourceLocator);
      }
    }
  }
}

void LuaScript::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  // Nothing to do?
}

unsigned int LuaScript::getArgumentIndex(const std::string& name) {
  for (unsigned int i = 0; i < cArguments.size(); i++) {
    std::string mName = cArguments[i]->getName();
    if (mName == name) {
      return i;
    }
  }
  std::cout << "No argument in script by the name of \"" << name << "\"" << std::endl;
  exit(1);
}

IArgumentDefinition* LuaScript::createArgumentDefinition(DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  std::string mType = node->getAttribute("type");
  if (mType == TYPE_SOUND) {
    return new ArgumentDefinitionReference(node, &TYPE_SOUND, resourceRegistry);
  } else if (mType == TYPE_INTEGER) {
    return new ArgumentDefinitionPrimitive(node, &TYPE_INTEGER, resourceRegistry);
  } else if (mType == TYPE_BOOLEAN) {
    return new ArgumentDefinitionPrimitive(node, &TYPE_BOOLEAN, resourceRegistry);
  } else if (mType == TYPE_FLOAT) {
    return new ArgumentDefinitionPrimitive(node, &TYPE_FLOAT, resourceRegistry);
  } else if (mType == TYPE_MODEL) {
    return new ArgumentDefinitionReference(node, &TYPE_MODEL, resourceRegistry);
  } else if (mType == TYPE_VERTEX) {
    return new ArgumentDefinitionReference(node, &TYPE_VERTEX, resourceRegistry);
  } else if (mType == TYPE_COLOUR) {
    return new ArgumentDefinitionReference(node, &TYPE_COLOUR, resourceRegistry);
  } else {
    return new ArgumentDefinitionReference(node, new std::string(mType), resourceRegistry);
  }
}

