/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "DumbIntegerValue.h"

DumbIntegerValue::DumbIntegerValue() {
  assignDummyPlugin(&cStringProcessor, "StringProcessor");
  cSockets.push_back(new PlugSocket("StringProcessor"));
  cValue = 0;
}

IIntegerValue& DumbIntegerValue::operator+=(const int& value) {
  cValue += value;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->valueChanged(cValue);
  }
  std::stringstream mStringStream;
  mStringStream << cValue;
  cText = mStringStream.str();
  std::cout << "Added " << value << " to score!" << std::endl;
  return *this;
}

void DumbIntegerValue::addIntegerValueListener(IIntegerValueListener* listener) {
  cListeners.push_back(listener);
}

void DumbIntegerValue::removeIntegerValueListener(IIntegerValueListener* listener) {
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    if (cListeners[i] == listener) {
      cListeners.erase(cListeners.begin() + i);
      return;
    }
  }
}

std::vector<PlugSocket*> DumbIntegerValue::getPlugSockets() {
  return cSockets;
}

void DumbIntegerValue::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "StringProcessor") {
    assignPlugin(plugin, &cStringProcessor, *socket);
    cStringProcessor->registerString(&cText);
  }
}

IPlugin* DumbIntegerValue::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "StringProcessor") {return cStringProcessor;}
  // TODO: Throw
  return NULL;
}

extern "C" IPlugin* create() {
  return new DumbIntegerValue();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
