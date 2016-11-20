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
#ifndef INTEGER_TO_STRING_H
#define INTEGER_TO_STRING_H

#include <string>

#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Utils.h>

#include "IString.h"

class IntegerToString:public IString {
  private:
  IInteger* cValue;

  public:
  IntegerToString(IInteger*);
  IInteger* getInteger();

  /**********************\
   * Implements IString *
  \**********************/
  std::string getValue();
  void setValue(const std::string&);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
};

#endif
