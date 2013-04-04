#ifndef ELEMENT_MODEL_TYPE_H
#define ELEMENT_MODEL_TYPE_H

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Resources/3DModel/I3DModelFactory.h>

#include "ElementModel.h"

class ElementModelType:public I3DModelFactory,
                       public IElementContainer {
  private:
  BlockLocation cIdentity;
  IElement* cElement;
  
  public:
  I3DModel* createModel(Vertex*, float);
  void destroyModel(I3DModel*);

  void save(DOMNodeWriter*, IResourceLocator*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void addElement(IElement*);
  void removeElement(IElement*);
};

#endif
