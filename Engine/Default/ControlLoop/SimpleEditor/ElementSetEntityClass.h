#ifndef ELEMENT_SET_ENTITY_CLASS_H
#define ELEMENT_SET_ENTITY_CLASS_H

#include <string>
#include <vector>

#include "../../../../Global/IElementSet.h"
#include "../../../../Global/ElementSetRegistry.h"
#include "../../../../Global/IComponentContainer.h"
#include "../../../../Global/PluginRegistry.h"
#include "../../../../Global/System.h"

#include "ConfirmationBox.h"
#include "IEntityClass.h"
#include "IPluginRegistryAccessor.h"
#include "PluginRequirementsComponent.h"

/**
 * This class provides a means to interface with element set instances.
 */
class ElementSetEntityClass:public IEntityClass {
  private:
  ElementSetRegistry* cElementSetRegistry;
  IPluginRegistryAccessor* cPluginRegistryAccessor;
  IComponentContainer* cComponentContainer;

  class RemoveCommand:public ICommand {
    private:
    ElementSetEntityClass* cParent;
    std::string cName;

    public:
    RemoveCommand(ElementSetEntityClass*, std::string);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:
  ElementSetEntityClass(ElementSetRegistry*, IPluginRegistryAccessor*, IComponentContainer*);

  /***************************\
   * Implements IEntityClass *
  \***************************/
  std::string getEntityClassName();
  void instantiate(std::string&, std::string&);
  void remove(std::string&);
  void configure(std::string&);
  std::vector<std::string*> getInstances();
  std::vector<std::string*> getImplementations();
};

#endif
