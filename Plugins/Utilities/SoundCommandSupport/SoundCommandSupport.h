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
#ifndef SOUND_COMMAND_SUPPORT_H
#define SOUND_COMMAND_SUPPORT_H

#include "../../SoundSupport/ISound.h"
#include "../../SoundSupport/ISoundSupport.h"
#include "../../SoundSupport/ISoundSupportListener.h"

#include "../IUtilities.h"

#include "PlaySoundCommand.h"

class SoundCommandSupport:public IUtilities,
                          public ISoundSupportListener {
  private:
  IRuntimeContext* cRuntimeContext;
  ISoundSupport* cSoundSupport;
  std::map<ISound*, PlaySoundCommand*> cCommands;

  public:
  SoundCommandSupport(IRuntimeContext*);

  /************************************\
   * Implements ISoundSupportListener *
  \************************************/
  void soundAdded(ISound*);
  void soundRemoved(ISound*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
};

#endif
