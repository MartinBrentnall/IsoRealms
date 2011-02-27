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
#ifndef ZONE_RENDERER_ENTITY_CLASS_H
#define ZONE_RENDERER_ENTITY_CLASS_H

#include <IsoRealms/ZoneRendererRegistry.h>

#include "IEntityClass.h"

class ZoneRendererEntityClass:public IEntityClass {
  private:
  ZoneRendererRegistry* cZoneRendererRegistry;
  
  public:
  ZoneRendererEntityClass(ZoneRendererRegistry*);

  /***************************\
   * Implements IEntityClass *
  \***************************/
  std::string getEntityClassName();
  void instantiate(std::string&, std::string&);
  void remove(std::string&);
  void configure(std::string&);
  std::vector<std::string*> getInstances();
  std::vector<std::string*> getImplementations();
};

#endif
