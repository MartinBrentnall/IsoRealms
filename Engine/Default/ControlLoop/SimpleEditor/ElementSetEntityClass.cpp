#include "ElementSetEntityClass.h"

ElementSetEntityClass::ElementSetEntityClass(ElementSetRegistry* elementSetRegistry, IPluginRegistryAccessor* pluginRegistryAccessor, IComponentContainer* componentContainer) {
  cElementSetRegistry = elementSetRegistry;
  cPluginRegistryAccessor = pluginRegistryAccessor;
  cComponentContainer = componentContainer;
}

std::string ElementSetEntityClass::getEntityClassName() {
  return "Element Set";
}

void ElementSetEntityClass::instantiate(std::string& type, std::string& name) {
  cElementSetRegistry->createInstance(type, name);
}

void ElementSetEntityClass::remove(std::string& name) {
  std::string mMessage = "Are you sure you want to remove \"" + name + "\"?  This operation will irreversibly destroy all elements of this set in your map!";
  std::string mTitle = "Confirm Remove Element Set";
  ICommand* mRemoveCommand = new RemoveCommand(this, name);
  IHUDComponent* mConfirmationBox = new ConfirmationBox(cComponentContainer, mRemoveCommand, mMessage, mTitle);
  cComponentContainer->addComponent(mConfirmationBox);
}

void ElementSetEntityClass::configure(std::string& name) {
  IElementSet* mElementSet = cElementSetRegistry->getElementSet(&name);
  IHUDComponent* mComponent = new PluginRequirementsComponent(cComponentContainer, cPluginRegistryAccessor, mElementSet, -0.8f, 0.0f);
  cComponentContainer->addComponent(mComponent);
}

std::vector<std::string*> ElementSetEntityClass::getInstances() {
  return cElementSetRegistry->getElementSets();
}

std::vector<std::string*> ElementSetEntityClass::getImplementations() {
  // TODO: Need to filter non directories.
  // TODO: Need to make path non-platform specific.
  std::vector<std::string>* mImplementationsList = System::getFileList("/usr/share/IsoRealms/Elements/", "*");
  std::vector<std::string*> mReturnList;
  for (unsigned int i = 0; i < mImplementationsList->size(); i++) {
    // TODO: This is BAD.  Memory leak here!
    mReturnList.push_back(new std::string((*mImplementationsList)[i]));
  }
  return mReturnList;
}

ElementSetEntityClass::RemoveCommand::RemoveCommand(ElementSetEntityClass* parent, std::string name) {
  cParent = parent;
  cName = name;
}

void ElementSetEntityClass::RemoveCommand::execute() {
  IElementSet* mElementSet = cParent->cElementSetRegistry->getElementSet(&cName);
  cParent->cElementSetRegistry->destroyInstance(mElementSet);
}

