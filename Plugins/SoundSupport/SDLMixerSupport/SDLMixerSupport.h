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
#ifndef SDL_MIXER_SUPPORT_H
#define SDL_MIXER_SUPPORT_H

#include "../../../Global/DefaultCommandInfo.h"

#include "../ISoundSupport.h"

#include "ConfigureSoundsCommand.h"
#include "ISoundRegistry.h"

class SDLMixerSupport:public ISoundSupport,
                      public ISoundRegistry {
  private:
  std::vector<ISound*> cSounds;
  std::vector<ISoundSupportListener*> cListeners;
  ConfigureSoundsCommand* cConfigureSoundsCommand;
  std::vector<ICommandInfo*> cPluginCommands;

  public:
  SDLMixerSupport();

  /*****************************\
   * Ipmlements ISoundRegistry *
  \*****************************/
  void addSound(ISound*);
  void removeSound(ISound*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);
  std::vector<ICommandInfo*> getCommandInfo();
  void setEditingContext(BlockLocation*, IComponentContainer*);

  /****************************\
   * Implements ISoundSupport *
  \****************************/
  std::vector<ISound*> getSounds();
  void addSoundSupportListener(ISoundSupportListener*);
  void removeSoundSupportListener(ISoundSupportListener*);
};

#endif
