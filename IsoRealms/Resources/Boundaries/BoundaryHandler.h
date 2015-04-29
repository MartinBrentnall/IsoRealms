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
#ifndef BOUNDARY_HANDLER_H
#define BOUNDARY_HANDLER_H

#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Script/IArgumentValueCollection.h>
#include <IsoRealms/Resources/Script/IScriptCall.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "IBoundaries.h"
#include "IBoundaryHandler.h"

class BoundaryHandler : public IBoundaryHandler {
  private:
  IBoundaries* cBoundaries;
  IScriptCall* cScriptEntered;
  IScriptCall* cScriptExited;
  
  public:
  BoundaryHandler(DOMNodeWrapper*, IResourceAccessor*, IBoundaries*);
   
  void save(DOMNodeWriter*, IResourceLocator*);
  void entered();
  void exited();  
  
  /*******************************\
   * Implements IBoundaryHandler * 
  \*******************************/
  void notifyMovement(IBoundaryPenetrator*, Vertex&, Vertex&);
  void notifyAppearance(IBoundaryPenetrator*, Vertex&);
  void notifyDisappearance(IBoundaryPenetrator*, Vertex&);
};

#endif
