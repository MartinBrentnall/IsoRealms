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
#ifndef I_SCRIPT_H
#define I_SCRIPT_H

#include <vector>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/IResources.h>

class IArgumentValue;
class IArgumentValueRegistry;
class IResources;

class IScript:public IResource {
  public:
    
  /**
   * 
   */
  virtual IScriptCall* createScriptCall(DOMNodeWrapper*, IArgumentValueRegistry*, IArgumentValueRegistry*) = 0;
    
  /**
   * 
   */
  virtual void execute(std::vector<IArgumentValue*>) = 0;
  
  // TODO: Seperate this to another interface.
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
  virtual void save(std::vector<IArgumentValue*>, DOMNodeWriter*, IResourceLocator*) = 0;
  
  virtual ~IScript() {}
};

#endif
