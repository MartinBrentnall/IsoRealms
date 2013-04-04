#ifndef SPINDIZZY_BLOCK_STATE_H
#define SPINDIZZY_BLOCK_STATE_H

#include <string>

#include <IsoRealms/ConditionElement.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelFactory.h>
#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class SpindizzyBlockState:public IResource {
  private:
  std::string cName;
  Vertex cClueModelLocation;
  I3DModel* cClueModel;
  ConditionElement* cState;
    
  public:
  SpindizzyBlockState();
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  bool* getInputAddress();
  I3DModel* getModel();
  ConditionElement* getConditionElement();
    
  void save(DOMNodeWriter*, IResourceLocator*);

  void setActive(bool);
  bool isActive();
};

#endif
