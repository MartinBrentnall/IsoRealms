#include "TimeLimit.h"

TimeLimit::TimeLimit() {
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  assignDummyPlugin(&cIntegerValue, "IntegerValue");
  cSockets.push_back(new PlugSocket("CommandRegistry"));
  cSockets.push_back(new PlugSocket("IntegerValue"));
  cMilliseconds = 180000;
  cMaximumMilliseconds = 180000;
  cValuePerSecond = 100;
}

IIntegerValue& TimeLimit::operator+=(const int& value) {
  cMilliseconds += value;
  if (cMilliseconds > cMaximumMilliseconds) {
    int mExcessMilliseconds = cMilliseconds - cMaximumMilliseconds;
    int mExcessValue = (mExcessMilliseconds * 0.001f) * cValuePerSecond;
    cIntegerValue += mExcessValue;
    cMilliseconds = cMaximumMilliseconds;
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
  std::cout << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setw(3) << mMilliseconds << std::endl;
}

std::string TimeLimit::getName() {
  return "Time Limit";
}

std::vector<PlugSocket*> TimeLimit::getPlugSockets() {
  return cSockets;
}

void TimeLimit::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    if (assignPlugin(plugin, &cCommandRegistry, *socket)) {
      cTimeOutCommands.clear();
    }
  } else if (socket->getType() == "IntegerValue") {
    assignPlugin(plugin, &cIntegerValue, *socket);
  }
}

IPlugin* TimeLimit::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  if (socket->getType() == "IntegerValue")    {return cIntegerValue;}
  // TODO: Throw
  return NULL;
}

extern "C" IPlugin* create() {
  return new TimeLimit();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
