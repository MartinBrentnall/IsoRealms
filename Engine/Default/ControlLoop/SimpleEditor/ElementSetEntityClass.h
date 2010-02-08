#ifndef ELEMENT_SET_ENTITY_CLASS_H
#define ELEMENT_SET_ENTITY_CLASS_H

#include <string>
#include <vector>

#include "IInstantiable.h"

#include "../../../../Global/IElementSet.h"
#include "../../../../Global/ElementSetRegistry.h"
#include "../../../../Global/IComponentContainer.h"
#include "../../../../Global/PluginRegistry.h"
#include "../../../../Global/System.h"

#include "PluginRequirementsComponent.h"

/**
 * This class provides a means to interface with element set instances.
 */
class ElementSetEntityClass:public IInstantiable {
  private:
  ElementSetRegistry* cElementSetRegistry;
  PluginRegistry* cPluginRegistry;
  IComponentContainer* cComponentContainer;

  public:
  ElementSetEntityClass(ElementSetRegistry*, PluginRegistry*, IComponentContainer*);

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
