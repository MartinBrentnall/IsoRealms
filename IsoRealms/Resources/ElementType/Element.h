/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef ELEMENT_H
#define ELEMENT_H

#include <IsoRealms/IModule.h>

#include "IElement.h"
#include "IElementType.h"

class Element:public IElement {
  public:
  virtual void updateRuntime(unsigned int milliseconds) {
  }
  
  virtual void renderEditing() {
    renderRuntime();
  }
  
  virtual void updateEditing(unsigned int milliseconds) {
    updateRuntime(milliseconds);
  }
  
  virtual void input(SDL_Event&) {
  }
  
  virtual bool isVisualRuntime() {
    return true;
  }
  
  virtual bool isVisualEditing() {
    return true;
  }
  
  virtual bool isDynamicRuntime() {
    return true;
  }
  
  virtual bool isDynamicEditing() {
    return true;
  }
  
  virtual bool isInteractive() {
    return false;
  }

  bool initElement(unsigned int) {
    return true;
  }
  
  virtual void removingElement() {
  };

  virtual void added() {
  }
  
  virtual void renderStaticEditing() {
  }

  virtual void initialiseResource(DOMNodeWrapper*, IResourceAccessor*) {
  }
  
  ~Element() {}
};

#endif
