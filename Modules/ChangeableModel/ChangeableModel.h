#ifndef CHANGEABLE_MODEL_H
#define CHANGEABLE_MODEL_H

#include <iostream>

#include <IsoRealms/Resources/3DModel/I3DModel.h>

class ChangeableModel:public I3DModel {
  private:
  I3DModel* cModel;
  
  public:
  ChangeableModel();
  
  void setModel(I3DModel*);
  
  /***********************\
   * Implements I3DModel *
  \***********************/
  void update(unsigned int);
  void render();
};

#endif
