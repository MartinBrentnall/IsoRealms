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

#include <vector>

class IElementFactory;
class IElementSet;
class IMap;

#include "BlockLocation.h"
#include "DOMNodeWriter.h"
#include "IDynamicElement.h"
#include "IElementContainer.h"
#include "IInteractiveElement.h"
#include "IVisualElement.h"

class IElement {
  public:

  /**
   * Retrieve the element set to which this element belongs.
   */
  virtual IElementSet* getElementSet() = 0;

  /**
   * Retrieve the element factory to which this element belongs.
   */
  virtual IElementFactory* getElementFactory() = 0;

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
  virtual bool initElement() = 0;

  /**
   * This function is called to notify the element that it's been removed from a
   * zone.  This is useful when the element has a relation with other elements
   * or plugins and wishes to perform some cleanup operations.
   */
  virtual void removed() = 0;

  /**
   * This function is called to notify the element that it's been re-added to a
   * zone.  This is useful when the element has a relation with other elements
   * or plugins and wishes to perform some initialization operations.
   * 
   * Note that this function should only be called after a call to removed().
   */
  virtual void added() = 0;

  virtual void setElementContainer(IElementContainer*) = 0;

  virtual void setRuntimeContext(IMap*) = 0;

  virtual void signalElementDirty() = 0;

  virtual void renderStatic() = 0;

  /**
   * This function can be called by editing tools to render things that should
   * only be shown to help with editing and not during the game.
   */
  virtual void renderStaticEditing() = 0;

  /**
   * Retrieve a list of interfaces through which this element is represented
   * visually.
   */
  virtual std::vector<IVisualElement*> getVisualElements() = 0;

  /**
   * Retrieve a list of interfaces through which this element can be updated.
   */
  virtual std::vector<IDynamicElement*> getDynamicElements() = 0;

  virtual std::vector<IDynamicElement*> getDynamicElementsRuntime() = 0;

  virtual std::vector<IInteractiveElement*> getInteractiveElements() = 0;

  virtual void save(DOMNodeWriter*, BlockLocation&) = 0;

  virtual ~IElement() {}
};

#endif
