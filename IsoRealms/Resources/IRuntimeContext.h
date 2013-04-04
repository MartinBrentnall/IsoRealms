/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef I_RUNTIME_CONTEXT_H
#define I_RUNTIME_CONTEXT_H

#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IComponentCustomType.h>
#include <IsoRealms/LuaSupport/IArgumentDefinitionType.h>
#include <IsoRealms/LuaSupport/IArgumentSource.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelFactory.h>
#include <IsoRealms/Resources/Boolean/IBoolean.h>
#include <IsoRealms/Resources/Collectables/ICollectables.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/ElementType/IElementGroupType.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/Sound/ISound.h>
#include <IsoRealms/Resources/SurfaceRegistry/ICollidableSurfaceRegistry.h>
#include <IsoRealms/Resources/Texture/ITexture.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "IResourceUseListener.h"

class IProject;

class IRuntimeContext {
  public:
  virtual IProject* getProject() = 0;
  virtual bool isEditing() = 0;
  virtual void add(IResource*,                                      DOMNodeWrapper* = NULL) = 0;
  virtual void add(IColour*,                    const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(ITexture*,                   const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(I3DModel*,                   const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(I3DModelFactory*,            const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(ISound*,                     const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IInteger*,                   const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IBoolean*,                   const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IFont*,                      const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IElementType*,               const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IHUDComponentFactory*,       const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(ISurfaceProcessor*,          const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(ICollidableSurfaceRegistry*, const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(ICollectables*,              const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IVertex*,                    const std::string&) = 0;
  virtual void add(ICamera*,                    const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IString*,                    const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IZoneHandler*,               const std::string&, DOMNodeWrapper* = NULL) = 0;
  virtual void add(IElementGroupType*,          const std::string&) = 0;
  virtual void add(IArgumentDefinitionType*,    const std::string&) = 0;
  virtual void add(IComponentCustomType*,       const std::string&) = 0;
  virtual void registerArgument(const std::string&, const std::string&, IArgumentSource*) = 0;
  virtual void addDynamicElement(IDynamicElement*) = 0;
  
  virtual void addListener(IResourceUseListener<IColour>*) = 0;
  virtual void addListener(IResourceUseListener<ITexture>*) = 0;
};

#endif
