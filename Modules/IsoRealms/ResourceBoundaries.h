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
#ifndef RESOURCE_BOUNDARIES_H
#define RESOURCE_BOUNDARIES_H

#include <map>
#include <SDL/SDL_mutex.h>
#include <sstream>
#include <vector>

#include <IsoRealms/LuaSupport/ArgumentValueProxy.h>
#include <IsoRealms/Resources/Boundaries/IBoundaries.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/Integer/Integer.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Resources/Script/IArgumentValueRegistry.h>
#include <IsoRealms/Resources/Script/IArgumentLocator.h>
#include <IsoRealms/Resources/Script/IScriptCall.h>
#include <IsoRealms/Struct/SpatialContainer2D.h>

class ResourceBoundaries:public IBoundaries,
                         public IArgumentValueRegistry,
                         public IArgumentLocator {
  private:
  SDL_mutex* cAccessMutex;
  IScriptCall* cEnteredScript;
  IScriptCall* cExitedScript;
//   ArgumentValue<IInteger> cArgumentZoneRemaining;
  SpatialContainer2D<IBoundary> cBoundaries;
  std::vector<IArgumentValueCollection*> cArgumentValuesBondaries;
  std::vector<IArgumentValueCollection*> cArgumentValuesBoundaryPenetrators;
  std::map<std::string, ArgumentValueProxy*> cArgumentValues;
  
  
//   unsigned int cCollectablesCount;
//   unsigned int cCollectedCount;
//   std::string cCollectedCountString;

  ArgumentValueProxy* getArgumentValue(const std::string&);
  void connectArguments(std::vector<IArgumentValueCollection*>, const std::string&);
  
  public:
  ResourceBoundaries(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
  
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /**************************\
   * Implements IBoundaries *
  \**************************/
  void registerArgumentValuesBoundaries(IArgumentValueCollection*);
  void registerArgumentValuesBoundaryPenetrator(IArgumentValueCollection*);
  void registerBoundary(IBoundary*);
  void notifyAppearance(IBoundaryPenetrator*, Vertex&);
  void notifyDisappearance(IBoundaryPenetrator*, Vertex&);
  void notifyMovement(IBoundaryPenetrator*, Vertex&, Vertex&);
  void reinitialise();
  
  IArgumentValue* getArgumentValue(DOMNodeWrapper*);
  std::string getPath(IArgumentValue*);
  
  virtual ~ResourceBoundaries() {}
};

#endif
