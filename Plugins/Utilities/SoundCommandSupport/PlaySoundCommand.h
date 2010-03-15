#ifndef PLAY_SOUND_COMMAND_H
#define PLAY_SOUND_COMMAND_H

#include "../../SoundSupport/ISound.h"
#include "../../CommandRegistry/IUserCommand.h"

class PlaySoundCommand:public IUserCommand {
  private:
  ISound* cSound;

  public:
  PlaySoundCommand(ISound*);

  /***************************\
   * Implements IUserCommand *
  \***************************/
  std::string getCommandName();
  void execute();
};

#endif
