#ifndef PLUGIN_ENTITY_CLASS_H
#define PLUGIN_ENTITY_CLASS_H

#include <string>

#include "../../../../Global/PluginRegistry.h"
#include "../../../../Global/IComponentContainer.h"

#include "ConfirmationBox.h"
#include "IEntityClass.h"
#include "IPluginRegistryAccessor.h"
#include "PluginRequirementsComponent.h"

class PluginEntityClass:public IEntityClass {
  private:
  IPluginRegistryAccessor* cPluginRegistryAccessor;
  std::string cPluginType;
  IComponentContainer* cComponentContainer;

  class RemoveCommand:public ICommand {
    private:
    PluginEntityClass* cParent;
    std::string cName;

    public:
    RemoveCommand(PluginEntityClass*, std::string);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:
  PluginEntityClass(IPluginRegistryAccessor*, std::string&, IComponentContainer*);

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
