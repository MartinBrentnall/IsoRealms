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
#include "ArgumentDefinitionReference.h"

ArgumentDefinitionReference::ArgumentDefinitionReference(DOMNodeWrapper* node, const std::string* type, IResourceRegistry* resourceRegistry) {
  cType = type;
  cName = node->getAttribute("name");
  cDefaultValue = new ArgumentValueProxy();
  resourceRegistry->add(cDefaultValue, node);
}

std::string ArgumentDefinitionReference::getName() {
  return cName;
}

std::string ArgumentDefinitionReference::getCode(const std::string& function, unsigned int arg) {
  std::string mFunction = "function " + function + "_arg" + Utils::toString(arg) + "(arg)\n";
  mFunction += "  " + cName + " = arg\n";
  mFunction += "end\n";
  mFunction += "\n";
  return mFunction;
}

std::string ArgumentDefinitionReference::getCleanup() {
  return "  " + cName + " = nil\n";
}

void ArgumentDefinitionReference::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("type", *cType);
  node->addAttribute("name", cName);
  if (cDefaultValue != NULL) {
    cDefaultValue->save(node, resourceLocator);
  }
}

IArgumentValue* ArgumentDefinitionReference::getDefaultValue() {
  return cDefaultValue;
}
