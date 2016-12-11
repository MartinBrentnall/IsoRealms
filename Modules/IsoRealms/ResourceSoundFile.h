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
#ifndef RESOURCE_SOUND_FILE_H
#define RESOURCE_SOUND_FILE_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/System.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Resources/Sound/ISound.h>

class ResourceSoundFile:public ISound {
  private:
  std::string cName;
  std::string cFile;
  Mix_Chunk* cSoundData;

  void loadData(const std::string&);
  
  public:
  ResourceSoundFile(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
  ResourceSoundFile(const std::string&, const std::string&);
    
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  Icon<ISound>* getResourceIcon(IResourceBrowser<ISound>*);
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  
  std::string getName();

  /*********************\
   * Implements ISound *
  \*********************/
  void play();

  virtual ~ResourceSoundFile() {}
};

#endif
