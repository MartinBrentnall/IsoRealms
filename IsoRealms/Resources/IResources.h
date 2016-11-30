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
#ifndef I_RESOURCES_H
#define I_RESOURCES_H

#include <IsoRealms/IProject.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/Float/IFloat.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceUseListener.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/Script/IGlobalVariable.h>
#include <IsoRealms/Resources/Script/IScript.h>
#include <IsoRealms/Resources/Sound/ISound.h>
#include <IsoRealms/Resources/String/IString.h>

#include "IResourceRegistry.h"
#include "Layer/ILayer.h"

class IResources:public IResourceAccessor {
  public:
  virtual void add(I3DModel*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(I3DModelType*,               std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
//  virtual void add(IArgumentDefinitionType*,    std::vector<std::string>, const std::string&) = 0;
  virtual void add(IArgumentValue*,             std::vector<std::string>, const std::string&, const std::string&) = 0;
  virtual void add(IBoolean*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IBoundaries*,                std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(ICamera*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IColour*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IElementType*,               std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IFont*,                      std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IGlobalVariable*,            std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IInteger*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(ILayerType*,                 std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IProject*,                   std::vector<std::string>, const std::string&) = 0;
  virtual void add(IResource*,                                                                DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IScript*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(ISound*,                     std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IString*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(ITexture*,                   std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  virtual void add(IVertex*,                    std::vector<std::string>, const std::string&, DOMNodeWrapper*, DOMNodeWrapper*) = 0;
  
  virtual void addDynamicElement(IDynamicElement*) = 0;
  
  virtual void addListener(IResourceUseListener<I3DModelType>*) = 0;
  virtual void addListener(IResourceUseListener<IBoolean>*) = 0;
  virtual void addListener(IResourceUseListener<IBoundaries>*) = 0;
  virtual void addListener(IResourceUseListener<ICamera>*) = 0;
  virtual void addListener(IResourceUseListener<IColour>*) = 0;
  virtual void addListener(IResourceUseListener<IElementType>*) = 0;
  virtual void addListener(IResourceUseListener<IFloat>*) = 0;
  virtual void addListener(IResourceUseListener<IFont>*) = 0;
  virtual void addListener(IResourceUseListener<IInteger>*) = 0;
  virtual void addListener(IResourceUseListener<ILayerType>*) = 0;
  virtual void addListener(IResourceUseListener<ISound>*) = 0;
  virtual void addListener(IResourceUseListener<IString>*) = 0;
  virtual void addListener(IResourceUseListener<ITexture>*) = 0;
  virtual void addListener(IResourceUseListener<IVertex>*) = 0;
};

#endif
