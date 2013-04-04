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

#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/SingleResourceTypeModule.h>

#include "ILock.h"

class TimeLimit:public IInteger,
                public IDynamicElement {
  private:
  int cMilliseconds;
  std::vector<ICommand*> cTimeOutCommands;
  ILock* cLock;

  public:
  TimeLimit(ILock*);
  
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  /****************************\
   * Implements IIntegerValue *
  \****************************/
  void setValue(int);
  int getValue();

  std::vector<IDynamicElement*> getPreLoopCommands();
  void save(DOMNodeWriter* node, IResourceLocator*);

  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(unsigned int);
};

#endif
