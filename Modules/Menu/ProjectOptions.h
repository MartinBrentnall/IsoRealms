/*
 * Copyright 2016 Martin Brentnall
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
#ifndef PROJECT_OPTIONS_H
#define PROJECT_OPTIONS_H

#include <map>
#include <string>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/IProjectOptions.h>

class ProjectOptions:public IProjectOptions {
  private:
  std::map<std::string, IModuleOptions*> cProjectOptions;
  
  class ModuleOptions:public IModuleOptions {
    private:
    std::map<std::string, std::string> cModuleOptions;
    std::map<std::string, IProjectOptions*> cSubProjectOptions;
    
    public:
    ModuleOptions(DOMNodeWrapper*, std::map<std::string, std::string>);
    
    /*****************************\
     * Implements IModuleOptions *
    \*****************************/
    std::string getOption(const std::string&);
    IProjectOptions* getProjectOptions(const std::string&);
  };
  
  public:
  ProjectOptions(DOMNodeWrapper*, std::map<std::string, std::string> = std::map<std::string, std::string>());

  /******************************\
   * Implements IProjectOptions *
  \******************************/
  IModuleOptions* getModuleOptions(const std::string&);
};

#endif
