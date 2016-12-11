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
#include "LuaScriptCall.h"

LuaScriptCall::LuaScriptCall(IScript* script, std::vector<IArgumentValue*> argumentValues) {
  cScript = script;
  cArgumentValues = argumentValues;
}

void LuaScriptCall::execute() {
  cScript->execute(cArgumentValues);
  std::cout << std::flush;
}

std::string LuaScriptCall::getScriptName() {
  return cScript->getName();
}

void LuaScriptCall::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  cScript->save(cArgumentValues, node, resourceLocator);
}
