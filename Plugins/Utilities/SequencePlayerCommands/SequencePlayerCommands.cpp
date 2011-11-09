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
  cPlayerCommands.push_back(new PlayCommand(this));
  cPlayerCommands.push_back(new RewindCommand(this));
  cPlayerCommands.push_back(new PauseCommand(this));
  cPlayerCommands.push_back(new UnpauseCommand(this));
  for (unsigned int i = 0; i < cPlayerCommands.size(); i++) {
    runtimeContext->add(cPlayerCommands[i]);
  }
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

std::string SequencePlayerCommands::PlayCommand::getName() {
  return "Play";
}

SequencePlayerCommands::RewindCommand::RewindCommand(SequencePlayerCommands* parent) {
  cParent = parent;
}

void SequencePlayerCommands::RewindCommand::execute() {
  cParent->cPlayer->rewind();
}

std::string SequencePlayerCommands::RewindCommand::getName() {
  return "Rewind";
}

SequencePlayerCommands::PauseCommand::PauseCommand(SequencePlayerCommands* parent) {
  cParent = parent;
}

void SequencePlayerCommands::PauseCommand::execute() {
  cParent->cPlayer->pause();
}

std::string SequencePlayerCommands::PauseCommand::getName() {
  return "Pause";
}

SequencePlayerCommands::UnpauseCommand::UnpauseCommand(SequencePlayerCommands* parent) {
  cParent = parent;
}

void SequencePlayerCommands::UnpauseCommand::execute() {
  cParent->cPlayer->unpause();
}

std::string SequencePlayerCommands::UnpauseCommand::getName() {
  return "Unpause";
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new SequencePlayerCommands(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
