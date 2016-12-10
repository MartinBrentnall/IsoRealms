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
#include "LuaGlobalVariable.h"

LuaGlobalVariable::LuaGlobalVariable(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cArgument = LuaScript::createArgumentDefinition(node, resourceRegistry);
}

void LuaGlobalVariable::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  DOMNodeWriter* mGlobalVariableNode = node->addBranch("GlobalVariable");
  cArgument->save(mGlobalVariableNode, resourceLocator);
}

void LuaGlobalVariable::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

void LuaGlobalVariable::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  std::string mFunction = cArgument->getCode("globalVar_" + cArgument->getName(), 0);
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->registerScript(mFunction);
  mConfiguration->setGlobalVariable(cArgument);
}
