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
#ifndef I_LAYER_H
#define I_LAYER_H

#include <SDL/SDL.h>

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IResourceLocator.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>

class ILayer : public virtual IResourceSelectionListener<IElementType> {
  public:
  virtual void updateRuntime(unsigned int) = 0;
  virtual void updateEditing(unsigned int) = 0;
  virtual void renderRuntime() = 0;
  virtual void renderEditing() = 0;
  virtual bool input(SDL_Event&) = 0;
  virtual bool inputEditor(SDL_Event&) = 0;
  virtual void initRuntime() = 0;
  virtual void initEditor() = 0;
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
  virtual void staticChanged() = 0;
  virtual void reset() = 0;
  
  virtual ~ILayer() {}
};

#endif
