#include "PluginEntityClass.h"

PluginEntityClass::PluginEntityClass(IPluginRegistryAccessor* pluginRegistryAccessor, std::string& pluginType, IComponentContainer* componentContainer) {
  cPluginRegistryAccessor = pluginRegistryAccessor;
  cPluginType = pluginType;
  cComponentContainer = componentContainer;
}

std::string PluginEntityClass::getEntityClassName() {
  return "\"" + cPluginType + "\" Plugin";
}

void PluginEntityClass::instantiate(std::string& type, std::string& instanceName) {
  PluginRegistry* mPluginRegistry = cPluginRegistryAccessor->getPluginRegistry();
  mPluginRegistry->loadPlugin(cPluginType, type, instanceName);
}

void PluginEntityClass::remove(std::string& name) {
  std::string mMessage = "Are you sure you want to remove \"" + name + "\"?";
  std::string mTitle = "Confirm Remove Plugin";
  ICommand* mRemoveCommand = new RemoveCommand(this, name);
  IHUDComponent* mConfirmationBox = new ConfirmationBox(cComponentContainer, mRemoveCommand, mMessage, mTitle);
  cComponentContainer->addComponent(mConfirmationBox);
}

void PluginEntityClass::configure(std::string& name) {
  PluginRegistry* mPluginRegistry = cPluginRegistryAccessor->getPluginRegistry();
  IPlugin* mPlugin = mPluginRegistry->getPlugin(cPluginType, name);
  IHUDComponent* mSupportedPluginsComponent = new PluginRequirementsComponent(cComponentContainer, cPluginRegistryAccessor, mPlugin, 0.05f, 0.05f);
  cComponentContainer->addComponent(mSupportedPluginsComponent);
}

std::vector<std::string*> PluginEntityClass::getInstances() {
  PluginRegistry* mPluginRegistry = cPluginRegistryAccessor->getPluginRegistry();
  std::vector<std::string> mInstanceNames = mPluginRegistry->getInstances(cPluginType);
  std::vector<std::string*> mReturnList;
  for (unsigned int i = 0; i < mInstanceNames.size(); i++) {
    // TODO: This is BAD.  Memory leak here!
    mReturnList.push_back(new std::string(mInstanceNames[i]));
  }
  return mReturnList;
}

std::vector<std::string*> PluginEntityClass::getImplementations() {
  // TODO: Need to filter non directories.
  // TODO: Need to make path non-platform specific.
  std::vector<std::string>* mImplementationsList = System::getFileList("/usr/share/IsoRealms/Plugins/" + cPluginType + "/", "*");
  std::vector<std::string*> mReturnList;
  for (unsigned int i = 0; i < mImplementationsList->size(); i++) {
    // TODO: This is BAD.  Memory leak here!
    mReturnList.push_back(new std::string((*mImplementationsList)[i]));
  }
  return mReturnList;
}

PluginEntityClass::RemoveCommand::RemoveCommand(PluginEntityClass* parent, std::string name) {
  cParent = parent;
  cName = name;
}

void PluginEntityClass::RemoveCommand::execute() {
  PluginRegistry* mPluginRegistry = cParent->cPluginRegistryAccessor->getPluginRegistry();
  IPlugin* mPlugin = mPluginRegistry->getPlugin(cParent->cPluginType, cName);
  mPluginRegistry->removePlugin(mPlugin);
}

