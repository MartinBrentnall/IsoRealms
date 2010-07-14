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

#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/IDynamicElement.h>

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
