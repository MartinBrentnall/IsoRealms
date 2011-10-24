#ifndef I_FRONT_END_COMMANDS_H
#define I_FRONT_END_COMMANDS_H

class IFrontEndCommands {
  public:
  virtual ICommand* parseCommand(DOMNodeWrapper*) = 0;
  virtual ICommand* parseArgumentCommand(DOMNodeWrapper*, const std::string&) = 0;
};

#endif
