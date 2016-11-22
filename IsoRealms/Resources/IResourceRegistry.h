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
#ifndef I_RESOURCE_REGISTRY_H
#define I_RESOURCE_REGISTRY_H

#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/Boolean/IBoolean.h>
#include <IsoRealms/Resources/Boundaries/IBoundaries.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/Layer/ILayerType.h>
#include <IsoRealms/Resources/Script/IGlobalVariable.h>
#include <IsoRealms/Resources/Script/IScript.h>
#include <IsoRealms/Resources/Sound/ISound.h>
#include <IsoRealms/Resources/Texture/ITexture.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "IResourceUseListener.h"

class IProject;

class IResourceRegistry {
  public:
  virtual bool isEditing() = 0;
  virtual void add(I3DModel*,                   const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(I3DModelType*,               const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IArgumentValue*,             const std::string&, const std::string&) = 0;
  virtual void add(IBoolean*,                   const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IBoundaries*,                const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(ICamera*,                    const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IColour*,                    const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IElementType*,               const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IFont*,                      const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IGlobalVariable*,            const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IInteger*,                   const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(ILayerType*,                 const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IScript*,                    const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IResource*,                                      DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(ISound*,                     const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IString*,                    const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(ITexture*,                   const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  virtual void add(IVertex*,                    const std::string&, DOMNodeWrapper* = nullptr, DOMNodeWrapper* = nullptr) = 0;
  
  virtual void addDynamicElement(IDynamicElement*) = 0;
  
  virtual void addListener(IResourceUseListener<IColour>*) = 0;
  virtual void addListener(IResourceUseListener<ITexture>*) = 0;
};

#endif
