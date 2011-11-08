/*
 * Copyright 2009,2010 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SoundCommandSupport.h"

SoundCommandSupport::SoundCommandSupport() {
  assignDummyPlugin(&cSoundSupport, "SoundSupport");
}

void SoundCommandSupport::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "SoundSupport") {
    ISoundSupport* mPreviousSoundSupport = cSoundSupport;
    if (assignPlugin(plugin, &cSoundSupport, *socket)) {
      mPreviousSoundSupport->removeSoundSupportListener(this);
      for (std::map<ISound*, PlaySoundCommand*>::iterator i = cCommands.begin(); i != cCommands.end(); i++) {
        cRuntimeContext->remove(i->second);
      }
      cCommands.clear();
      std::vector<ISound*> mSounds = cSoundSupport->getSounds();
      for (unsigned int i = 0; i < mSounds.size(); i++) {
        PlaySoundCommand* mPlaySoundCommand = new PlaySoundCommand(mSounds[i]);
        cCommands[mSounds[i]] = mPlaySoundCommand;
        cRuntimeContext->add(mPlaySoundCommand);
      }
      cSoundSupport->addSoundSupportListener(this);
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* SoundCommandSupport::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SoundSupport")    {return cSoundSupport;}
  // TODO: Throw
  return NULL;
}

void SoundCommandSupport::soundAdded(ISound* sound) {
  PlaySoundCommand* mPlaySoundCommand = new PlaySoundCommand(sound);
  cCommands[sound] = mPlaySoundCommand;
  cRuntimeContext->add(mPlaySoundCommand);
}

void SoundCommandSupport::soundRemoved(ISound* sound) {
  cCommands.erase(sound);
}

void SoundCommandSupport::setRuntimeContext(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
}

extern "C" IPlugin* create() {
  return new SoundCommandSupport();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
