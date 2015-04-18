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
#ifndef RESOURCE_STRING_TIME_H
#define RESOURCE_STRING_TIME_H

#include <iomanip>
#include <sstream>

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Resources/String/IString.h>

class ResourceStringTime:public IString {
  private:
  IInteger* cValue;
  
  public:
  ResourceStringTime(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
    
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /**********************\
   * Implements IString *
  \**********************/
  std::string getValue();
  void setValue(const std::string&);
    
  virtual ~ResourceStringTime() {}
};

#endif
