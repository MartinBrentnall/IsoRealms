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
#ifndef I_ELEMENT_TYPE_H
#define I_ELEMENT_TYPE_H

#include <SDL/SDL.h>

class IElement;

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/ILayerEditingContext.h>
#include <IsoRealms/IResourceSource.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "IElement.h"
#include "IElementContainer.h"

class IModule;

/**
 * The element type creates engine elements.
 */
class IElementType:public IResource {
  public:
  /**
   * Parse an element immediately from a DOM node.
   *
   * @param DOMNodeWrapper*  The node to parse from.
   * @returns  The parsed element.
   */
  virtual void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool) = 0;
  
  /**
   * Signal to the type that the user wishes to configure the element.  This
   * can be typically used to push a configuration component into the
   * configured component container.
   */
  virtual void configureElement() = 0;

  /**
   * Enable user input for configuring the type.
   */
  virtual bool inputEdit(SDL_Event&, ILayerEditingContext*) = 0;

  /**
   * Render a preview representation of the element to be created by this
   * type.  This representation lies in 3D map space.
   */
  virtual void renderEditingPreview(Vertex&) = 0;

  virtual void updateEditingPreview(unsigned int) = 0;
  
  /**
   * Render an iconic representation of the element to be created by this
   * type.  This representation should fit inside -1 to +1 on each
   * axis.
   */
  virtual void renderIcon() = 0;

  /**
   * Update the iconic representation of the element to be created by this
   * type.
   * 
   * @param int  Time passed in milliseconds since last update.
   */
  virtual void updateIcon(unsigned int) = 0;

  /**
   * Destroy the element.  Make sure you've removed the element from your map
   * before calling this!  You should only call this function with an element
   * created by this element set!  Failure to comply by this rule will result
   * in undefined behaviour and possible destruction of the universe.
   * 
   * @param IElement*  The element to destroy.
   */
  virtual void destroy(IElement*) = 0;
  
  virtual void removeElement(IElement*) = 0;
  
  /**
   * Called when the editor cursor has stopped.  This is used if the element
   * wants to adjust the cursor position (e.g. to align to a grid).
   * 
   * @param Vertex*  The cursor location.
   * @return         Adjusted cursor location.
   */
  virtual Vertex* editorCursorStopped(Vertex*) = 0;
  
  virtual ~IElementType() {}
};

#endif
