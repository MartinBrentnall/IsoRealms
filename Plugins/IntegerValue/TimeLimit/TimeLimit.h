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
#ifndef TIME_LIMIT_H
#define TIME_LIMIT_H

#include <iomanip>
#include <sstream>

#include <IsoRealms/IDynamicElement.h>

#include "../../StringProcessor/IStringProcessor.h"

#include "../IIntegerValue.h"

class TimeLimit:public IIntegerValue,
                public IDynamicElement {
  private:
  int cMilliseconds;
  int cMaximumMilliseconds;
  int cValuePerSecond;
  IRuntimeContext* cRuntimeContext;
  IIntegerValue* cIntegerValue;
  IStringProcessor* cStringProcessor;
  std::vector<ICommand*> cTimeOutCommands;
  unsigned int cLocks;
  std::vector<ICommand*> cCommands;
  std::string cText;

  class LockControlCommand:public ICommand {
    private:
    TimeLimit* cParent;
    bool cLock;
    
    public:
    LockControlCommand(TimeLimit*, bool);
    
    /***************************\
     * Implements ICommand *
    \***************************/
    void execute();
  };
  
  public:
  TimeLimit(IRuntimeContext*);

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
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
};

#endif
