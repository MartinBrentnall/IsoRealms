/*
 * Copyright 2015 Martin Brentnall
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
#ifndef I_RESOURCE_MANAGER_H
#define I_RESOURCE_MANAGER_H

#include "IResourceListener.h"
#include "IResourceType.h"

class IDialogGenerator;

class IResourceManager:public IResourceLocator {
  public:
  virtual std::vector<I3DModelType*>         getAllModelTypes() = 0;
  virtual std::vector<IBoolean*>             getAllBooleans() = 0;
  virtual std::vector<IBoundaries*>          getAllBoundaries() = 0;
  virtual std::vector<ICamera*>              getAllCameras() = 0;
  virtual std::vector<IColour*>              getAllColours() = 0;
  virtual std::vector<IElementType*>         getAllElementTypes() = 0;
  virtual std::vector<IFloat*>               getAllFloats() = 0;
  virtual std::vector<IFont*>                getAllFonts() = 0;
  virtual std::vector<IInteger*>             getAllIntegers() = 0;
  virtual std::vector<IScript*>              getAllScripts() = 0;
  virtual std::vector<ISound*>               getAllSounds() = 0;
  virtual std::vector<IString*>              getAllStrings() = 0;
  virtual std::vector<ITexture*>             getAllTextures() = 0;
  virtual std::vector<IVertex*>              getAllVertices() = 0;
  
  virtual void addResourceListener(IResourceListener<I3DModelType>*) = 0;          
  virtual void addResourceListener(IResourceListener<IBoolean>*) = 0;                 
  virtual void addResourceListener(IResourceListener<IBoundaries>*) = 0;        
  virtual void addResourceListener(IResourceListener<ICamera>*) = 0;                  
  virtual void addResourceListener(IResourceListener<IColour>*) = 0;
  virtual void addResourceListener(IResourceListener<IElementType>*) = 0;         
  virtual void addResourceListener(IResourceListener<IFloat>*) = 0;               
  virtual void addResourceListener(IResourceListener<IFont>*) = 0;                
  virtual void addResourceListener(IResourceListener<IInteger>*) = 0;
  virtual void addResourceListener(IResourceListener<IScript>*) = 0;
  virtual void addResourceListener(IResourceListener<ISound>*) = 0;           
  virtual void addResourceListener(IResourceListener<IString>*) = 0;          
  virtual void addResourceListener(IResourceListener<ITexture>*) = 0;         
  virtual void addResourceListener(IResourceListener<IVertex>*) = 0;          
  
  virtual void editResource(I3DModelType*,         IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IBoolean*,             IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IBoundaries*,          IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(ICamera*,              IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IColour*,              IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IElementType*,         IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IFloat*,               IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IFont*,                IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IInteger*,             IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IScript*,              IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(ISound*,               IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IString*,              IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(ITexture*,             IResourceAccessor*, IEditingContext*) = 0;
  virtual void editResource(IVertex*,              IResourceAccessor*, IEditingContext*) = 0;

  virtual void removeResource(I3DModelType*,         IResourceAccessor*) = 0;
  virtual void removeResource(IBoolean*,             IResourceAccessor*) = 0;
  virtual void removeResource(IBoundaries*,          IResourceAccessor*) = 0;
  virtual void removeResource(ICamera*,              IResourceAccessor*) = 0;
  virtual void removeResource(IColour*,              IResourceAccessor*) = 0;
  virtual void removeResource(IElementType*,         IResourceAccessor*) = 0;
  virtual void removeResource(IFloat*,               IResourceAccessor*) = 0;
  virtual void removeResource(IFont*,                IResourceAccessor*) = 0;
  virtual void removeResource(IInteger*,             IResourceAccessor*) = 0;
  virtual void removeResource(IScript*,              IResourceAccessor*) = 0;
  virtual void removeResource(ISound*,               IResourceAccessor*) = 0;
  virtual void removeResource(IString*,              IResourceAccessor*) = 0;
  virtual void removeResource(ITexture*,             IResourceAccessor*) = 0;
  virtual void removeResource(IVertex*,              IResourceAccessor*) = 0;
  
  virtual std::vector<IDialogGenerator*> getDialogGenerators() = 0;
};

#endif
