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

#include <luabind/luabind.hpp>

#include <IsoRealms/DefaultCommandInfo.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/OpenDialogCommand.h>

#include "DialogAudioFileSelector.h"
#include "ISoundRegistry.h"

class SDLMixerSupport:public IPlugin,
                      public IResourceType<ISound>,
		      public IConfirmationListener {
  private:
  IRuntimeContext* cRuntimeContext;
  std::vector<DialogAudioFileSelector*> cCreateDialogs;
  std::vector<SDLMixerSound*> cSounds;
  IComponentContainer* cComponentContainer;

  public:
  SDLMixerSupport();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void save(DOMNodeWriter*, IResourceLocator*);
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void setEditingContext(IEditingContext*, IResourceManager*);

  /*********************************\
   * Implements ISoundResourceType *
  \*********************************/
  void createResource(IResourceAccessor*);
  void editResource(ISound*, IResourceAccessor*);
  void removeResource(ISound*, IResourceAccessor*, IResourceInstanceListener<ISound>*);
  
  /************************************\
   * Implements IConfirmationListener *
  \************************************/
  void dialogConfirmed(IRectangularComponent*);
  void dialogCancelled(IRectangularComponent*);
};

#endif
