#ifndef I_TOGGLE_SWITCH_COMMAND_H
#define I_TOGGLE_SWITCH_COMMAND_H

#include <IsoRealms/ICommand.h>

class IToggleSwitchCommand:public ICommand {
  public:
  virtual void save(DOMNodeWriter*) = 0;
};

#endif