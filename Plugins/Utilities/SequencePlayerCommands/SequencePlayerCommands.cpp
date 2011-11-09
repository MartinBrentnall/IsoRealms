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
#include "SequencePlayerCommands.h"

SequencePlayerCommands::SequencePlayerCommands(IRuntimeContext* runtimeContext) {
  assignDummyPlugin(&cPlayer, "SequencePlayer");
  runtimeContext->add(new PlayCommand(this),    "Play");
  runtimeContext->add(new RewindCommand(this),  "Rewind");
  runtimeContext->add(new PauseCommand(this),   "Pause");
  runtimeContext->add(new UnpauseCommand(this), "Unpause");
}

IPlugin* SequencePlayerCommands::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SequencePlayer") {return cPlayer;}
  // TODO: Throw
  return NULL;
}

void SequencePlayerCommands::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "SequencePlayer") {
    assignPlugin(plugin, &cPlayer, *socket);
  } else {
    // TODO: Throw
  }
}

SequencePlayerCommands::PlayCommand::PlayCommand(SequencePlayerCommands* parent) {
  cParent = parent;
}

void SequencePlayerCommands::PlayCommand::execute() {
  cParent->cPlayer->play();
}

SequencePlayerCommands::RewindCommand::RewindCommand(SequencePlayerCommands* parent) {
  cParent = parent;
}

void SequencePlayerCommands::RewindCommand::execute() {
  cParent->cPlayer->rewind();
}

SequencePlayerCommands::PauseCommand::PauseCommand(SequencePlayerCommands* parent) {
  cParent = parent;
}

void SequencePlayerCommands::PauseCommand::execute() {
  cParent->cPlayer->pause();
}

SequencePlayerCommands::UnpauseCommand::UnpauseCommand(SequencePlayerCommands* parent) {
  cParent = parent;
}

void SequencePlayerCommands::UnpauseCommand::execute() {
  cParent->cPlayer->unpause();
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new SequencePlayerCommands(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
