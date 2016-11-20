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
#ifndef COMPONENT_RESOURCE_TEXTURE_SELECTOR_H
#define COMPONENT_RESOURCE_TEXTURE_SELECTOR_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <IsoRealms/Resources/IResourceSelectionListener.h>
#include <IsoRealms/Resources/IResourceSelector.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "IComponentSelectorListener.h"
#include "ISelector.h"
#include "SelectableComponent.h"

class ComponentResourceTextureSelector:public ISelector,
                                       public IResourceSelectionListener<ITexture> {
  private:
  IComponentSelectorListener<ITexture>* cParent;
  IResourceSelector* cResourceSelector;
  ITexture* cTexture;
  
  public:
  ComponentResourceTextureSelector(IComponentSelectorListener<ITexture>*, ITexture*, IResourceSelector*);
  
  /************************\
   * Implements ISelector *
  \************************/
  void render(SelectableComponent*);
  void selected();
  void deselected();
  
  void resourceSelected(ITexture*);
};
    
#endif
