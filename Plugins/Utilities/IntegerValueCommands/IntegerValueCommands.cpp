#include "IntegerValueCommands.h"

IntegerValueCommands::IntegerValueCommands() {
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  assignDummyPlugin(&cVariable, "IntegerValue");
  cSockets.push_back(new PlugSocket("CommandRegistry"));
  cSockets.push_back(new PlugSocket("IntegerValue"));
}

void IntegerValueCommands::addCommand(AddIntegerCommand* command) {
  cCommands.push_back(command);
  cCommandRegistry->registerCommand(command);
}

void IntegerValueCommands::removeCommand(AddIntegerCommand* command) {
  cCommandRegistry->registerCommand(command);
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    if (cCommands[i] == command) {
      cCommands.erase(cCommands.end() + i);
      return;
    }
  }
  // TODO: Chuck
  std::cout << "WARNING: Removed command wasn't found in local registry!" << std::endl;
}

std::vector<AddIntegerCommand*> IntegerValueCommands::getCommands() {
  return cCommands;
}

void IntegerValueCommands::save(DOMNodeWriter* node) {
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    DOMNodeWriter* mCommandNode = node->addBranch("Command");
    cCommands[i]->save(mCommandNode);
  }
}

void IntegerValueCommands::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      AddIntegerCommand* mCommand = new AddIntegerCommand(mNode, cVariable);
      addCommand(mCommand);
    }
  }
}

std::string IntegerValueCommands::getName() {
  return "Integer Value Commands";
}

std::vector<PlugSocket*> IntegerValueCommands::getPlugSockets() {
  return cSockets;
}

void IntegerValueCommands::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    ICommandRegistry* mPreviousCommandRegistry = cCommandRegistry;
    if (assignPlugin(plugin, &cCommandRegistry, *socket)) {
      for (unsigned int i = 0; i < cCommands.size(); i++) {
        mPreviousCommandRegistry->unregisterCommand(cCommands[i]);
        cCommandRegistry->registerCommand(cCommands[i]);
      }
    }
  } else if (socket->getType() == "IntegerValue") {
    if (assignPlugin(plugin, &cVariable, *socket)) {
      for (unsigned int i = 0; i < cCommands.size(); i++) {
        cCommands[i]->setVariable(cVariable);
      }
    }
  } else {
    // TODO: Chuck
  }
}

IPlugin* IntegerValueCommands::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  if (socket->getType() == "IntegerValue")    {return cVariable;}
  // TODO: chuck
  return NULL;
}

extern "C" IPlugin* create() {
  return new IntegerValueCommands();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
