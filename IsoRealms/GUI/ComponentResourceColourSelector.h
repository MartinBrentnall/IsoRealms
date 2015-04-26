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
#ifndef COMPONENT_RESOURCE_COLOUR_SELECTOR_H
#define COMPONENT_RESOURCE_COLOUR_SELECTOR_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>
#include <IsoRealms/Resources/IResourceSelector.h>

#include "IComponentSelectorListener.h"
#include "ISelector.h"
#include "SelectableComponent.h"

class ComponentResourceColourSelector:public ISelector,
                                      public IResourceSelectionListener<IColour> {
  private:
  IComponentSelectorListener<IColour>* cParent;
  IResourceSelector* cResourceSelector;
  IColour* cBorrowedColour;
  IColour* cColour;
  
  public:
  ComponentResourceColourSelector(IComponentSelectorListener<IColour>*, IColour*, IResourceSelector*);
  
  /************************\
   * Implements ISelector *
  \************************/
  void render(SelectableComponent*);
  void selected();
  void deselected();
  
  void resourceSelected(IColour*);
};
    
#endif
