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
#ifndef I_RESOURCE_LOCATOR_H
#define I_RESOURCE_LOCATOR_H

#include <string>

class DOMNodeWriter;
class I3DModel;
class I3DModelType;
class IArgumentValue;
class IBoolean;
class IBoundaries;
class ICamera;
class IColour;
class IComponentCustomType;
class IElement;
class IElementGroupType;
class IElementType;
class IFloat;
class IFont;
class IGeometryProcessor;
class IGlobalVariable;
class IHUDComponentType;
class IInteger;
class ILayerType;
class IScript;
class IScriptCall;
class ISound;
class IString;
class ISurfaceRegistry;
class ITexture;
class IVertex;

class IResourceLocator {
  public:
  virtual std::string getPath(I3DModel*) = 0;
  virtual std::string getPath(I3DModelType*) = 0;
  virtual std::string getPath(IArgumentValue*) = 0;
  virtual std::string getPath(IBoolean*) = 0;
  virtual std::string getPath(IBoundaries*) = 0;
  virtual std::string getPath(ICamera*) = 0;
  virtual std::string getPath(IColour*) = 0;
  virtual std::string getPath(IComponentCustomType*) = 0;
  virtual std::string getPath(IElementGroupType*) = 0;
  virtual std::string getPath(IElementType*) = 0;
  virtual std::string getPath(IFloat*) = 0;
  virtual std::string getPath(IFont*) = 0;
  virtual std::string getPath(IGeometryProcessor*) = 0;
  virtual std::string getPath(IGlobalVariable*) = 0;
  virtual std::string getPath(IHUDComponentType*) = 0;
  virtual std::string getPath(IInteger*) = 0;
  virtual std::string getPath(ILayerType*) = 0;
  virtual std::string getPath(IScript*) = 0;
  virtual std::string getPath(ISound*) = 0;
  virtual std::string getPath(IString*) = 0;
  virtual std::string getPath(ISurfaceRegistry*) = 0;
  virtual std::string getPath(ITexture*) = 0;
  virtual std::string getPath(IVertex*) = 0;
  
  virtual void saveScript(DOMNodeWriter*, const std::string&, IScriptCall*) = 0;
  
  virtual bool isImplicit(IElement*) = 0;
};

#endif
