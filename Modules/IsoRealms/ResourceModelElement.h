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
#ifndef RESOURCE_MODEL_ELEMENT_H
#define RESOURCE_MODEL_ELEMENT_H

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/ElementType/IElementContainer.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>

#include "ModelElement.h"

class ResourceModelElement:public I3DModelType,
                           public IElementContainer,
                           public IUniverse {
  private:
  BlockLocation cIdentity;
  IElement* cElement;
  ModelElement* cSampleModel;
  
  public:
  ResourceModelElement(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
    
  I3DModel* createModel(Vertex*, float);
  void destroyModel(I3DModel*);

  void save(DOMNodeWriter*, IResourceLocator*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void updateIcon(unsigned int);
  void renderIcon();
  
  void addElement(IElement*);
  void removeElement(IElement*);
  void updateElement(IElement*);
  void addArgumentValue(IArgument*);
  void setArguments();
  void unsetArguments();
  BlockArea* getCoverage();
  void setDirty(IElement*);
  void restrictCursor(Vertex&);
  IUniverse* getUniverse();
  
  virtual ~ResourceModelElement() {}
};

#endif
