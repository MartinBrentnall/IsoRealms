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
#ifndef ELEMENT_SET_REGISTRY_H
#define ELEMENT_SET_REGISTRY_H

#include <dlfcn.h>
#include <map>
#include <vector>

#include "BlockLocation.h"
#include "CommandDirectory.h"
#include "CommandRegistryProxy.h"
#include "IElementRegistryListener.h"
#include "IElementSet.h"
#include "IElementSetRegistry.h"
#include "InitException.h"
#include "PluginRegistry.h"
#include "System.h"

class ElementSetRegistry:public IElementSetRegistry {
  private:

  /**
   * Listeners are notified when element set instances are instantiated and
   * destroyed.
   */
  std::vector<IElementRegistryListener*> cListeners;

  /**
   * Instantiated element sets by instance name.
   */
  std::map<std::string, IElementSet*> cElementSets;

  /**
   * Each plug-in maps to its corresponding destroy function.
   */
  std::map<std::string, destroyElementSet*> cDestroyFunctions;

  /**
   * Map each element set instance name to its corresponding library.
   */
  std::map<std::string, void*> cSOHandles;

  /**
   * Map each instance name to its implementation type.
   */
  std::map<std::string, std::string> cElementSetTypes;

  int indexOf(IElementRegistryListener*);
  void loadElementSet(std::string);
  void unloadElementSet(std::string);
  bool exists(std::string);

  public:
  ElementSetRegistry();

  void registerElementSet(DOMNodeWrapper*, CommandDirectory*, IMap*, bool);
  void connectPlugin(PluginRegistry*, DOMNodeWrapper*);
  void loadConfiguration(DOMNodeWrapper*);

  void setPlugin(PluginRegistry*, IPluginSupport*, DOMNodeWrapper*);

  std::vector<IElement*> loadElements(DOMNodeWrapper*, BlockLocation*, IElementContainer*);

  /**
   * Create an element set instance of the specified type and assign it the
   * specified name.  The instance name must be a unique name to this
   * element set type.
   * 
   * @param string  Element set implementation to create.
   * @param string  Instance name to assign to the new element set.
   * @return  The new element set instance.
   */
  IElementSet* createInstance(std::string, std::string);

  /**
   * Remove the specified element set instance from the registry.
   * 
   * TODO: Who is responsible for clean-up?  Should we expect clean-up to occur from events to the listeners, or should it be done before hand?
   */ 
  void destroyInstance(IElementSet*);

  /**
   * Add an element registry listener.  The listener will be notified when
   * element set instances are created and destroyed.
   * 
   * @param IElementRegistryListener*  The listener to add.
   */
  void addElementRegistryListener(IElementRegistryListener*);

  /**
   * Remove an element registry listener.
   * 
   * @param IElementRegistryListener*  The listener to remove.
   */
  void removeElementRegistryListener(IElementRegistryListener*);

  std::vector<std::string*> getElementSets();

  std::string getInstanceName(IElementSet*);

  void setEditingInfo(BlockLocation*, IEditingContext*, IElementGateway*, IComponentContainer*, CommandDirectory*);

  /**
   *
   */
  IElementSet* getElementSet(std::string*);

  /**
   * Calling this function will cause all element sets in this registry to
   * cease using the specified plug-in.
   */
  void pluginRemoved(IPlugin*);

  void save(PluginRegistry*, DOMNodeWriter*);

  /**
   * This method is called to notify that all elements have been initialized
   * TODO: The specification of this function probably needs to be more detailed.
   */
  void initElementsComplete();
  
  /**********************************\
   * Implements IElementSetRegistry *
  \**********************************/
  std::string getEntityPath(IElementSet*);

  ~ElementSetRegistry();
};

#endif
