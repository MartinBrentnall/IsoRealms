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
#ifndef RESOURCE_MODEL_SET_CYCLEABLE_H
#define RESOURCE_MODEL_SET_CYCLEABLE_H

#include <IsoRealms/LuaSupport/ArgumentValueCustomType.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceRegistry.h>

#include "IModelTypeCycleable.h"
#include "ModelCycleable.h"

class ResourceModelSetCycleable : public IResource {
  private:
  class ModelCycleableOffset : public I3DModelType,
                               public IModelTypeCycleable {
    private:
    ResourceModelSetCycleable* cParent;
    std::vector<ModelCycleable*> cModels;
    unsigned int cOffset;
   
    public:
    ModelCycleableOffset(ResourceModelSetCycleable*, unsigned int);
    
    void updateModel(I3DModelType*, unsigned int);
   
    /**********************************\
     * Implements IModelTypeCycleable *
    \**********************************/
    unsigned int getCycleIndex();
   
    /***************************\
     * Implements I3DModelType *
    \***************************/
    void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
    I3DModel* createModel(Vertex*, float);
    void destroyModel(I3DModel*);
    void renderIcon();
    void updateIcon(unsigned int);
  };
 
  std::string cModelSetName;
  std::vector<I3DModelType*> cModelTypes;
  std::vector<ModelCycleableOffset*> cCycleableModels;
  unsigned int cCycleIndex = 0;

  public:
  ResourceModelSetCycleable(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
 
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*);  
  std::vector<std::string> getResourceNames();
  I3DModelType* getResource(const std::string&);
  std::vector<I3DModelType*> getResources();
  bool contains(I3DModelType*);
  std::string stripMemberName(const std::string&);
  
  /***********************\
   * Scripting Interface *
  \***********************/
  void next();
  void previous();
  
  virtual ~ResourceModelSetCycleable() {}
};

#endif
