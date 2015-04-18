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
#include "ArgumentDefinitionPrimitive.h"

ArgumentDefinitionPrimitive::ArgumentDefinitionPrimitive(DOMNodeWrapper* node, const std::string* type, IResourceRegistry* resourceRegistry) {
  cType = type;
  cName = node->getAttribute("name");
  cDefaultValue = new ArgumentValueProxy();
  resourceRegistry->add(cDefaultValue, node);
}

std::string ArgumentDefinitionPrimitive::getName() {
  return cName;
}

std::string ArgumentDefinitionPrimitive::getCode(const std::string& function, unsigned int arg) {
  std::string mFunction = "function " + function + "_arg" + Utils::toString(arg) + "(arg)\n";
  mFunction += "  _" + cName + " = arg\n";
  mFunction += "  " + cName + " = arg:getValue()\n";
  mFunction += "end\n";
  mFunction += "\n";
  return mFunction;
}

std::string ArgumentDefinitionPrimitive::getCleanup() {
  std::string mFunction = "  _" + cName + ":setValue(" + cName + ")\n";
  mFunction += "  _" + cName + " = nil\n";
  mFunction += "  " + cName + " = nil\n";
  return mFunction;
}

void ArgumentDefinitionPrimitive::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("type", *cType);
  node->addAttribute("name", cName);
  if (cDefaultValue != NULL) {
    cDefaultValue->save(node, resourceLocator);
  }
}  

IArgumentValue* ArgumentDefinitionPrimitive::getDefaultValue() {
  return cDefaultValue;
}
