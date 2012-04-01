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
#ifndef ZONE_RENDERER_PROXY_H
#define ZONE_RENDERER_PROXY_H

#include <string>

#include "CommandProxy.h"
#include "IZoneRenderer.h"
#include "RegistryProxy.h"

class ZoneRendererProxy:public IZoneRenderer {
  private:
  IZoneRenderer* cZoneRenderer;
  bool cActive;
  std::string cName;
  std::string cInstance;
  std::string cType;
  
  class RendererActivationCommand:public ICommand {
    private:
    ZoneRendererProxy* cParent;
    bool cActivate;
    
    public:
    RendererActivationCommand(ZoneRendererProxy*, bool);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  public:
  ZoneRendererProxy(IZoneRenderer*, bool, RegistryProxy<ICommand, CommandProxy>*, const std::string&, const std::string&, const std::string&);

  void save(DOMNodeWriter*);
  
  /****************************\
   * Implements IZoneRenderer *
  \****************************/
  void render(std::vector<IZone*>&, IProject*);
  void update(std::vector<IZone*>&, unsigned int);
  void updateRuntime(std::vector<IZone*>&, unsigned int);
};

#endif
