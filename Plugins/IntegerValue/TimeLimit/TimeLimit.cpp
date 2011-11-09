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
#include "TimeLimit.h"

TimeLimit::TimeLimit(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  assignDummyPlugin(&cIntegerValue, "IntegerValue");
  assignDummyPlugin(&cStringProcessor, "StringProcessor");
  cMilliseconds = 180000;
  cMaximumMilliseconds = 180000;
  cValuePerSecond = 100;
  cLocks = 0;
  cRuntimeContext->add(new LockControlCommand(this, true), "AddLock");
  cRuntimeContext->add(new LockControlCommand(this, false), "RemoveLock");
}

IIntegerValue& TimeLimit::operator+=(const int& value) {
  cMilliseconds += value;
  if (cMilliseconds > cMaximumMilliseconds) {
    int mExcessMilliseconds = cMilliseconds - cMaximumMilliseconds;
    int mExcessValue = (mExcessMilliseconds * 0.001f) * cValuePerSecond;
    (*cIntegerValue) += mExcessValue;
    cMilliseconds = cMaximumMilliseconds;
  } else if (cMilliseconds < 0) {
    cMilliseconds = 0;
  }
  return *this;
}

void TimeLimit::addIntegerValueListener(IIntegerValueListener*) {
  // Nothing to do.  Listeners not supported.
}

void TimeLimit::removeIntegerValueListener(IIntegerValueListener*) {
  // Nothing to do.  Listeners not supported.
}

std::vector<IDynamicElement*> TimeLimit::getPreLoopCommands() {
  std::vector<IDynamicElement*> mTimeCommand;
  mTimeCommand.push_back(this);
  return mTimeCommand;
}

void TimeLimit::update(int milliseconds) {
  if (cLocks == 0) {
    if (cMilliseconds > 0) {
      cMilliseconds -= milliseconds;
      if (cMilliseconds <= 0) {
        for (unsigned int i = 0; i < cTimeOutCommands.size(); i++) {
          cTimeOutCommands[i]->execute();
        }
        cMilliseconds = 0;
      }
    }
    int mMilliseconds = cMilliseconds % 1000;
    int mSeconds = cMilliseconds / 1000;
    int mMinutes = mSeconds / 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setw(1) << mMilliseconds;
    cText = mStringStream.str();
  }
}

void TimeLimit::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "IntegerValue") {
    assignPlugin(plugin, &cIntegerValue, *socket);
  } else if (socket->getType() == "StringProcessor") {
    assignPlugin(plugin, &cStringProcessor, *socket);
    cStringProcessor->registerString(&cText);
  }
}

IPlugin* TimeLimit::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "IntegerValue")    {return cIntegerValue;}
  if (socket->getType() == "StringProcessor") {return cStringProcessor;}
  // TODO: Throw
  return NULL;
}

TimeLimit::LockControlCommand::LockControlCommand(TimeLimit* parent, bool lock) {
  cParent = parent;
  cLock = lock;
}

void TimeLimit::LockControlCommand::execute() {
  cParent->cLocks += cLock ? 1 : -1;
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new TimeLimit(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
