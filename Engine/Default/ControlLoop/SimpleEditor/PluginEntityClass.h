#ifndef PLUGIN_ENTITY_CLASS_H
#define PLUGIN_ENTITY_CLASS_H

#include <string>

#include "IInstantiable.h"
#include "PluginRequirementsComponent.h"

#include "../../../../Global/PluginRegistry.h"
#include "../../../../Global/IComponentContainer.h"

class PluginEntityClass:public IInstantiable {
  private:
  PluginRegistry* cPluginRegistry;
  std::string cPluginType;
  IComponentContainer* cComponentContainer;

  public:
  PluginEntityClass(PluginRegistry*, std::string&, IComponentContainer*);

  /*************************\
   * Implements IInstiable *
  \*************************/
  std::string getEntityClassName();
  void instantiate(std::string&, std::string&);
  void configure(std::string&);
  std::vector<std::string*> getInstances();
  std::vector<std::string*> getImplementations();
};

#endif
