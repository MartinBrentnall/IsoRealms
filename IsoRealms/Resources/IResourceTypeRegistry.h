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
#ifndef I_RESOURCE_TYPE_REGISTRY_H
#define I_RESOURCE_TYPE_REGISTRY_H

#include <string>

#include <IsoRealms/Resources/IResourceType.h>

class I3DModelType;
class IBoolean;
class IBoundaries;
class ICamera;
class IColour;
class IComponentCustomType;
class IElementType;
class IFloat;
class IFont;
class IGeometryProcessor;
class IHUDComponentType;
class IInteger;
class IScript;
class ISound;
class IString;
class ISurfaceRegistry;
class ITexture;
class IVertex;

class IResourceTypeRegistry {
  public:
  virtual void addResourceType(IResourceType<I3DModelType>*,         const std::string&) = 0;
  virtual void addResourceType(IResourceType<IBoolean>*,             const std::string&) = 0;
  virtual void addResourceType(IResourceType<IBoundaries>*,          const std::string&) = 0;
  virtual void addResourceType(IResourceType<ICamera>*,              const std::string&) = 0;
  virtual void addResourceType(IResourceType<IColour>*,              const std::string&) = 0;
  virtual void addResourceType(IResourceType<IComponentCustomType>*, const std::string&) = 0;
  virtual void addResourceType(IResourceType<IElementType>*,         const std::string&) = 0;
  virtual void addResourceType(IResourceType<IFloat>*,               const std::string&) = 0;
  virtual void addResourceType(IResourceType<IFont>*,                const std::string&) = 0;
  virtual void addResourceType(IResourceType<IGeometryProcessor>*,   const std::string&) = 0;
  virtual void addResourceType(IResourceType<IHUDComponentType>*,    const std::string&) = 0;
  virtual void addResourceType(IResourceType<IInteger>*,             const std::string&) = 0;
  virtual void addResourceType(IResourceType<ILayerType>*,           const std::string&) = 0;
  virtual void addResourceType(IResourceType<IScript>*,              const std::string&) = 0;
  virtual void addResourceType(IResourceType<ISound>*,               const std::string&) = 0;
  virtual void addResourceType(IResourceType<IString>*,              const std::string&) = 0;
  virtual void addResourceType(IResourceType<ISurfaceRegistry>*,     const std::string&) = 0;
  virtual void addResourceType(IResourceType<ITexture>*,             const std::string&) = 0;
  virtual void addResourceType(IResourceType<IVertex>*,              const std::string&) = 0;
};

#endif
