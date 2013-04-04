#ifndef ELEMENT_MODEL_H
#define ELEMENT_MODEL_H

#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/ElementType/IElement.h>
#include <IsoRealms/IsoRealmsConstants.h>

class ElementModel:public I3DModel {
  private:
  IElement** cElement;
  Vertex* cLocation;
  float cScale;
  std::vector<IDynamicElement*> cDynamics;
  std::vector<IVisualElement*> cVisuals;
    
  public:
  ElementModel(IElement**, Vertex*, float);
  
  void update(unsigned int);
  void render();
};

#endif
