#ifndef SCRIPTABLE_VERTICES_H
#define SCRIPTABLE_VERTICES_H

#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/IPlugin.h>

#include "ScriptableVertex.h"

class ScriptableVertices:public IPlugin {
  private:
  std::vector<ScriptableVertex*> cVertices;
  
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
