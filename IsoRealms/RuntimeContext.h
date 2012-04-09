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
#include "I3DModel.h"
#include "I3DModelFactory.h"
#include "IColour.h"
#include "IResources.h"
#include "IRuntimeContext.h"
#include "ModelFactoryProxy.h"
#include "RegistryProxy.h"
#include "TextureProxy.h"

class RuntimeContext:public IRuntimeContext {
  private:
  IResources* cResources;
  std::vector<std::string> cEntityPath;
  
  public:
  RuntimeContext(IResources*, std::vector<std::string>);

  /******************************\
   * Implements IRuntimeContext *
  \******************************/
  IProject* getProject();
  bool isEditing();
  std::string getLocation(ICommand*); // TODO: What's this for?  Is it used?
  void add(ICommand*, const std::string&);
  void add(IColour*, const std::string&);
  void add(ITexture*, const std::string&);
  void add(I3DModelFactory*, const std::string&);
  void add(ISound*, const std::string&);
  void add(IInteger*, const std::string&);
  void remove(ICommand*);
  IScript* getLuaScript(DOMNodeWrapper*);
  Script* getScript(DOMNodeWrapper*);
  IColour* getColour(DOMNodeWrapper*);
  ITexture* getTexture(DOMNodeWrapper*);
  I3DModel* getModel(DOMNodeWrapper*, Vertex*);
  I3DModel* getModel(const std::string&, Vertex*);
  ISound* getSound(DOMNodeWrapper*);
};

#endif
