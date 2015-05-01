#ifndef MODEL_CYCLEABLE_H
#define MODEL_CYCLEABLE_H

#include <vector>

#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>

#include "IModelTypeCycleable.h"

class ModelCycleable : public I3DModel {
  private:
  IModelTypeCycleable* cModelType;
  std::vector<I3DModel*> cModels;
  Vertex* cLocation;
  float cScale;
 
  public:
  ModelCycleable(IModelTypeCycleable*, std::vector<I3DModelType*>, Vertex*, float);
 
  void setModel(I3DModelType*, unsigned int);
  
  /***********************\
   * Implements I3DModel *
  \***********************/
  void render();
  void update(unsigned int);
};

#endif
