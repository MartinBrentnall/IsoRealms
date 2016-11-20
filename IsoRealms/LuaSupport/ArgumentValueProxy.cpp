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
#include "ArgumentValueProxy.h"

ArgumentValueProxy::ArgumentValueProxy() {
  cArgumentValue = nullptr;
}

void ArgumentValueProxy::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  cArgumentValue = resourceAccessor->getArgumentValue(node);
}

void ArgumentValueProxy::setArgumentValue(IArgumentValue* argumentValue) {
  cArgumentValue = argumentValue;
}

void ArgumentValueProxy::setArgument(lua_State* luaState, const std::string& name) {
  cArgumentValue->setArgument(luaState, name);
}

void ArgumentValueProxy::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  if (cArgumentValue != nullptr) {
    cArgumentValue->save(node, resourceLocator);
  }
}

bool ArgumentValueProxy::isDefaultArgument() {
  return cArgumentValue->isDefaultArgument();
}
