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
#ifndef I_RESOURCE_ACCESSOR_H
#define I_RESOURCE_ACCESSOR_H

#include <string>
#include <vector>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/ElementType/IElementContainer.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

class IArgumentValue;
class IArgumentValueRegistry;
class IArgumentDefinition;
class I3DModel;
class I3DModelType;
class IBoundaries;
class ICamera;
class IColour;
class IComponentCustomType;
class IFloat;
class IFont;
class IHUDComponentType;
class IInteger;
class IProject;
class IScriptCall;
class ISound;
class IString;
class ITexture;
class IVertex;

class IResourceAccessor {
  public:
  virtual bool isEditing() = 0;
  
  virtual bool*                 getDigitalInput(       const std::string&) = 0;
  virtual IArgumentValue*       getArgumentValue(      DOMNodeWrapper*) = 0;
  virtual IArgumentValue*       getArgumentValueCustom(DOMNodeWrapper*) = 0;
  virtual I3DModel*             getModel(              const std::string&, Vertex*, float = 1.0f) = 0;
  virtual I3DModelType*         getModelType(          const std::string&) = 0;
  virtual IBoolean*             getBoolean(            const std::string&) = 0;
  virtual IBoundaries*          getBoundaries(         const std::string&) = 0;
  virtual ICamera*              getCamera(             const std::string&) = 0;
  virtual IColour*              getColour(             const std::string&) = 0;
  virtual IComponentCustomType* getComponentCustomType(const std::string&) = 0;
  virtual IFloat*               getFloat(              const std::string&) = 0;
  virtual IFont*                getFont(               const std::string&) = 0;
  virtual IHUDComponentType*    getHUDComponentType(   const std::string&) = 0;
  virtual IInteger*             getInteger(            const std::string&) = 0;
  virtual ISound*               getSound(              const std::string&) = 0;
  virtual IString*              getString(             const std::string&) = 0;
  virtual ITexture*             getTexture(            const std::string&) = 0;
  virtual IVertex*              getVertex(             const std::string&) = 0;
  
  virtual void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*) = 0;
  virtual IScriptCall* getScriptCall(DOMNodeWrapper*, IArgumentValueRegistry* = NULL) = 0;
//  virtual IArgumentDefinition* getRegisteredArgumentDefinition(DOMNodeWrapper*) = 0;
//  virtual IArgumentValue* createArgumentValue(DOMNodeWrapper*) = 0;
  
  // TODO: Not sure if this interface is the right place for this
  virtual void notifyChange(IColour*) = 0;
  virtual void notifyChange(ITexture*) = 0;
};

#endif
