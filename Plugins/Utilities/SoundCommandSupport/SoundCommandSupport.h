#ifndef SOUND_COMMAND_SUPPORT_H
#define SOUND_COMMAND_SUPPORT_H

#include "../../CommandRegistry/ICommandRegistry.h"
#include "../../SoundSupport/ISound.h"
#include "../../SoundSupport/ISoundSupport.h"
#include "../../SoundSupport/ISoundSupportListener.h"

#include "../IUtilities.h"

#include "PlaySoundCommand.h"

class SoundCommandSupport:public IUtilities,
                          public ISoundSupportListener {
  private:
  std::vector<PlugSocket*> cSockets;
  ICommandRegistry* cCommandRegistry;
  ISoundSupport* cSoundSupport;
  std::map<ISound*, PlaySoundCommand*> cCommands;

  public:
  SoundCommandSupport();

  /************************************\
   * Implements ISoundSupportListener *
  \************************************/
  void soundAdded(ISound*);
  void soundRemoved(ISound*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
};

#endif
