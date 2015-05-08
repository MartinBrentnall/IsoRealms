/*
 * Copyright 2009 Martin Brentnall
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
#ifndef I_ELEMENT_H
#define I_ELEMENT_H

#include <SDL/SDL.h>
#include <vector>

class IElementType;
class IElementSet;
class IMap;

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/ILayerEditingContext.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/IResourceLocator.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "IElementBounds.h"

class PickedElement;

class IElement {
  public:

  /**
   * Retrieve the element type to which this element belongs.
   */
  virtual IElementType* getElementType() = 0;

  /**
   * Initialise the element.
   * ... and...
   * Test whether the element is ready for the runtime.  A return value of
   * false will cause initElement to be called to attempt to initialise the
   * element.  The runtime initialization process will then continue to
   * perform initialization on all other elements that require it before
   * returning to this one and repeating the process.
   * 
   * @returns  True to indicate that the element is ready to be used in the
   *           runtime, otherwise false to indicate that a call to initElement
   *           is required.
   */
  virtual bool initElement(unsigned int) = 0;

  virtual void renderStatic() = 0;
  
  virtual void renderRuntime() = 0;
  
  virtual void renderEditing() = 0;
  
  virtual void updateRuntime(unsigned int) = 0;
  
  virtual void updateEditing(unsigned int) = 0;
  
  virtual void input(SDL_Event&) = 0;

  virtual bool isVisualRuntime() = 0;
  
  virtual bool isVisualEditing() = 0;

  virtual bool isDynamicRuntime() = 0;

  virtual bool isDynamicEditing() = 0;

  virtual bool isInteractive() = 0;

  virtual void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&) = 0;

  virtual void setDirty() = 0;

  virtual void initRuntime() {} // TODO: Should be pure
  
  virtual void staticChanged() {} // TODO: Should be pure
  
  virtual IElementBounds* getBounds() = 0;
  
  virtual void focusGained(ILayerEditingContext*) = 0;
  virtual void focusLost(ILayerEditingContext*) = 0;
  virtual void cursorMoved(ILayerEditingContext*, Vertex&, Vertex&) = 0;
  virtual void cursorAppeared(ILayerEditingContext*, Vertex&) = 0;
  virtual void processCursorMovement(ILayerEditingContext*, Vertex&, Vertex&) = 0;
  virtual void processCursorAppearance(ILayerEditingContext*, Vertex&) = 0;
  virtual PickedElement* pickElement(Vertex&, Vertex&) = 0;
  
  virtual ~IElement() {}
};

#endif
