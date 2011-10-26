#ifndef I_TOGGLE_SWITCH_COMMAND_H
#define I_TOGGLE_SWITCH_COMMAND_H

class IToggleSwitchCommand:public IUserCommand {
  public:
  virtual void save(DOMNodeWriter*) = 0;
};

#endif