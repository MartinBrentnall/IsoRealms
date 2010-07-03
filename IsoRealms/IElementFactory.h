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
#ifndef I_ELEMENT_FACTORY_H
#define I_ELEMENT_FACTORY_H

#include <SDL/SDL.h>

class IElement;

#include "BlockLocation.h"
#include "DOMNodeWrapper.h"
#include "IComponentContainer.h"
#include "IElement.h"
#include "IElementContainer.h"
#include "IElementGateway.h"

/**
 * The element factory creates engine elements.
 */
class IElementFactory {
  public:
  virtual IElementSet* getElementSet() = 0;

  // Runtime and parsing functions.
  /**
   * Parse an element from a DOM node.
   *
   * @param DOMNodeWrapper*  The node to parse from.
   * @returns  The parsed element.
   */
  virtual IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*) = 0;

  /**
   * Retrieve the unique identifying name of this factory.
   * 
   * @returns  Unique identifying name of this factory.
   */
  virtual std::string getName() = 0;

  // Editor functions.
  /**
   * Set a pointer to the editing location within the map.
   * 
   * @param BlockLocation*  The editing location within the map.
   * @param IElementGateway*  The gateway through which to push new elements.
   */
  virtual void setEditingContext(BlockLocation*, IElementGateway*, IComponentContainer*) = 0;

  /**
   * Signal to the factory that the user wishes to configure the element.  This
   * can be typically used to push a configuration component into the
   * configured component container.
   */
  virtual void configureElement() = 0;

  /**
   * Enable user input for configuring the factory.
   */
  virtual bool input(SDL_Event&) = 0;

  /**
   * Render a preview representation of the element to be created by this
   * factory.  This representation lies in 3D map space.
   */
  virtual void renderEditingPreview() = 0;

  /**
   * Render an iconic representation of the element to be created by this
   * factory.  This representation should fit inside -1 to +1 on each
   * axis.
   */
  virtual void renderIcon() = 0;

  /**
   * Update the iconic representation of the element to be created by this
   * factory.
   * 
   * @param int  Time passed in milliseconds since last update.
   */
  virtual void updateIcon(int) = 0;

  virtual ~IElementFactory() {}
};

#endif
