#ifndef MODEL_PROXY_H
#define MODEL_PROXY_H

#include <iostream>

#include "I3DModelFactory.h"
#include "IsoRealmsConstants.h"

class ModelFactoryProxy:public I3DModelFactory {
  private:
  I3DModelFactory* cModelFactory;

  class DummyModel:public I3DModel {
    private:
    Vertex* cLocation;
    float cScale;
    
    public:
    DummyModel(Vertex*, float);
      
    /***********************\
     * Implements I3DModel *
    \***********************/
    void render();
    void update(int);
  };
  
  public:
  ModelFactoryProxy();
  
  void setImplementation(I3DModelFactory*);

  /******************************\
   * Implements I3DModelFactory *
  \******************************/
  I3DModel* createModel(Vertex*, float = 1.0f);
  void destroyModel(I3DModel*);
};

#endif
