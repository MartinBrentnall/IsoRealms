#ifndef ELEMENT_MODEL_H
#define ELEMENT_MODEL_H

#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/ElementType/IElement.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class ElementModel:public I3DModel {
  private:
  IElement** cElement;
  Vertex* cLocation;
  float cScale;
    
  public:
  ElementModel(IElement**, Vertex*, float);
  
  void update(unsigned int);
  void render();
};

#endif
