#ifndef TIME_LIMIT_H
#define TIME_LIMIT_H

#include <iomanip>

#include "../../../Global/IDynamicElement.h"

#include "../../CommandRegistry/ICommandRegistry.h"

#include "../IIntegerValue.h"

class TimeLimit:public IIntegerValue,
                public IDynamicElement {
  private:
  int cMilliseconds;
  int cMaximumMilliseconds;
  int cValuePerSecond;
  ICommandRegistry* cCommandRegistry;
  IIntegerValue* cIntegerValue;
  std::vector<IUserCommand*> cTimeOutCommands;
  std::vector<PlugSocket*> cSockets;

  public:
  TimeLimit();

  /****************************\
   * Implements IIntegerValue *
  \****************************/
  IIntegerValue& operator+=(const int&);
  void addIntegerValueListener(IIntegerValueListener*);
  void removeIntegerValueListener(IIntegerValueListener*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  std::vector<IDynamicElement*> getPreLoopCommands();

  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(int);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
};

#endif
