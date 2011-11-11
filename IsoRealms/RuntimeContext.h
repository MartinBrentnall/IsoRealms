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
#ifndef RUNTIME_CONTEXT_H
#define RUNTIME_CONTEXT_H

#include "ColourProxy.h"
#include "CommandProxy.h"
#include "IColour.h"
#include "IColourSource.h"
#include "IRuntimeContext.h"
#include "IScriptSource.h"
#include "ITextureSource.h"
#include "RegistryProxy.h"
#include "TextureProxy.h"

class RuntimeContext:public IRuntimeContext {
  private:
  IMap* cMap;
  RegistryProxy<ICommand, CommandProxy>* cCommandGateway;
  RegistryProxy<IColour, ColourProxy>* cColourGateway;
  RegistryProxy<ITexture, TextureProxy>* cTextureGateway;
  bool cEditing;
  IScriptSource* cScriptSource;
  IColourSource* cColourSource;
  ITextureSource* cTextureSource;
  
  public:
  RuntimeContext(IMap*, RegistryProxy<ICommand, CommandProxy>*, RegistryProxy<IColour, ColourProxy>*, RegistryProxy<ITexture, TextureProxy>*, bool, IScriptSource*, IColourSource*, ITextureSource*);

  /******************************\
   * Implements IRuntimeContext *
  \******************************/
  IMap* getMap();
  bool isEditing();
  std::string getLocation(ICommand*); // TODO: What's this for?  Is it used?
  void add(ICommand*, const std::string&);
  void add(IColour*, const std::string&);
  void add(ITexture*, const std::string&);
  void remove(ICommand*);
  Script* getScript(DOMNodeWrapper*);
  IColour* getColour(DOMNodeWrapper*);
  ITexture* getTexture(DOMNodeWrapper*);
};

#endif
