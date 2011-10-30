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
#ifndef I_ELEMENT_SET_H
#define I_ELEMENT_SET_H

#include <vector>

class IElementFactory;

#include "BlockLocation.h"
#include "DOMNodeWriter.h"
#include "IAddressableEntity.h"
#include "ICommandRegistry.h"
#include "IComponentContainer.h"
#include "IElementFactory.h"
#include "IElementGateway.h"
#include "IPluginSupport.h"
#include "IRuntimeContext.h"

class IElementSetRegistry;

/**
 * TODO: Write a description of this interface
 */
class IElementSet:public IPluginSupport,
                  public virtual IAddressableEntity {
  public:

  /**
   * Retrieves a list of the factories for creating the elements of this set.
   * 
   * @returns  The factories for creating elements of this set.
   */
  virtual std::vector<IElementFactory*> getElementFactories() = 0;

  // TODO: Enable this function for element sets with variable number of element factories.
  // virtual void addElementSetListener(IElementSetListener*) = 0;

  /**
   * Save the configuration of this element set to the specified node.
   */
  virtual void save(DOMNodeWriter*) {}

  virtual void load(DOMNodeWrapper*) {}

  /**
   * Set a pointer to the editing location within the map.
   * 
   * @param BlockLocation*  The editing location within the map.
   * @param IElementGateway*  The gateway through which to push new elements.
   */
  virtual void setEditingContext(BlockLocation*, IElementGateway*, IComponentContainer*) = 0;

  virtual void setRuntimeContext(IRuntimeContext*) = 0;
  
  /**
   * Destroy the element.  Make sure you've removed the element from your map
   * before calling this!  You should only call this function with an element
   * created by this element set!  Failure to comply by this rule will result
   * in undefined behaviour and possible crashes.
   * 
   * @param IElement*  The element to destroy.
   */
  virtual void destroy(IElement*) = 0;

  /**
   * TODO
   */
  virtual void setElementSetRegistry(IElementSetRegistry*) = 0;
  
  /**
   * This method is called to notify that all elements have been initialized
   * TODO: The specification of this function probably needs to be more detailed.
   */
  virtual void initElementsComplete() {}
  
  /**
   * Destroy the element set.  Do not call this until all elements from this
   * set have been destroyed!
   */
  virtual ~IElementSet() {}
};

typedef IElementSet* createElementSet(DOMNodeWrapper*);
typedef void destroyElementSet(IElementSet*);

#endif
