#include "SoundCommandSupport.h"

SoundCommandSupport::SoundCommandSupport() {
  assignDummyPlugin(&cCommandRegistry, "CommandRegistry");
  assignDummyPlugin(&cSoundSupport, "SoundSupport");
  cSockets.push_back(new PlugSocket("CommandRegistry"));
  cSockets.push_back(new PlugSocket("SoundSupport"));
}

std::string SoundCommandSupport::getName() {
  return "Sound Command Support";
}

std::vector<PlugSocket*> SoundCommandSupport::getPlugSockets() {
  return cSockets;
}

void SoundCommandSupport::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "CommandRegistry") {
    ICommandRegistry* mPreviousCommandRegistry = cCommandRegistry;
    if (assignPlugin(plugin, &cCommandRegistry, *socket)) {
      for (std::map<ISound*, PlaySoundCommand*>::iterator i = cCommands.begin(); i != cCommands.end(); i++) {
        mPreviousCommandRegistry->unregisterCommand(i->second);
        cCommandRegistry->registerCommand(i->second);
      }
    }
  } else if (socket->getType() == "SoundSupport") {
    ISoundSupport* mPreviousSoundSupport = cSoundSupport;
    if (assignPlugin(plugin, &cSoundSupport, *socket)) {
      mPreviousSoundSupport->removeSoundSupportListener(this);
      for (std::map<ISound*, PlaySoundCommand*>::iterator i = cCommands.begin(); i != cCommands.end(); i++) {
        cCommandRegistry->unregisterCommand(i->second);
      }
      cCommands.clear();
      std::vector<ISound*> mSounds = cSoundSupport->getSounds();
      for (unsigned int i = 0; i < mSounds.size(); i++) {
        PlaySoundCommand* mPlaySoundCommand = new PlaySoundCommand(mSounds[i]);
        cCommands[mSounds[i]] = mPlaySoundCommand;
        cCommandRegistry->registerCommand(mPlaySoundCommand);
      }
      cSoundSupport->addSoundSupportListener(this);
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* SoundCommandSupport::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CommandRegistry") {return cCommandRegistry;}
  if (socket->getType() == "SoundSupport")    {return cSoundSupport;}
  // TODO: Throw
  return NULL;
}

void SoundCommandSupport::soundAdded(ISound* sound) {
  PlaySoundCommand* mPlaySoundCommand = new PlaySoundCommand(sound);
  cCommands[sound] = mPlaySoundCommand;
}

void SoundCommandSupport::soundRemoved(ISound* sound) {
  cCommands.erase(sound);
}

extern "C" IPlugin* create() {
  return new SoundCommandSupport();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
