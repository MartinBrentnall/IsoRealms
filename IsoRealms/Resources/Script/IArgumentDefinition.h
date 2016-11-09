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
#ifndef I_ARGUMENT_DEFINITION_H
#define I_ARGUMENT_DEFINITION_H

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/IResourceLocator.h>

class IArgumentValue;

class IArgumentDefinition {
  public:
  virtual std::string getName() = 0;
  virtual std::string getCode(const std::string&, unsigned int) = 0;
  virtual std::string getCleanup() = 0;
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
  virtual void saveCall(DOMNodeWriter*, IResourceLocator*) = 0;
  virtual IArgumentValue* getDefaultValue() = 0;
  virtual bool isDefaultValue(IArgumentValue*) = 0;
};

#endif
