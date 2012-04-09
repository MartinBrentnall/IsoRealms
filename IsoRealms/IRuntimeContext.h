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

#include "Configuration.h"
#include "DOMNodeWrapper.h"
#include "I3DModel.h"
#include "I3DModelFactory.h"
#include "IColour.h"
#include "IInteger.h"
#include "ILuaScript.h"
#include "ISound.h"
#include "ITexture.h"
#include "Script.h"
#include "Vertex.h"

class IProject;

class IRuntimeContext {
  public:
  virtual IProject* getProject() = 0;
  virtual bool isEditing() = 0;
  virtual std::string getLocation(ICommand*) = 0; // TODO: What's this for?  Is it used?
  virtual void add(ICommand*, const std::string&) = 0;
  virtual void add(IColour*, const std::string&) = 0;
  virtual void add(ITexture*, const std::string&) = 0;
  virtual void add(I3DModelFactory*, const std::string&) = 0;
  virtual void add(ISound*, const std::string&) = 0;
  virtual void add(IInteger*, const std::string&) = 0;
  virtual void remove(ICommand*) = 0;
  virtual ILuaScript* getLuaScript(const std::string&) = 0;
  virtual Script* getScript(DOMNodeWrapper*) = 0;
  virtual IColour* getColour(DOMNodeWrapper*) = 0;
  virtual ITexture* getTexture(DOMNodeWrapper*) = 0;
  virtual I3DModel* getModel(DOMNodeWrapper*, Vertex*) = 0;
  virtual I3DModel* getModel(const std::string&, Vertex*) = 0;
};

#endif
