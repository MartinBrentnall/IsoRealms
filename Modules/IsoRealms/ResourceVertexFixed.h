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
#ifndef RESOURCE_VERTEX_FIXED_H
#define RESOURCE_VERTEX_FIXED_H

#include <IsoRealms/LuaSupport/ArgumentValueCustomType.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class ResourceVertexFixed:public IVertex {
  private:
  Vertex cVertex;
  
  public:
  ResourceVertexFixed(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
    
  IVertex* getVertex();
    
  double getX();
  double getY();
  double getZ();
  void setX(double);
  void setY(double);
  void setZ(double);

  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  
  virtual ~ResourceVertexFixed() {}
};

#endif
