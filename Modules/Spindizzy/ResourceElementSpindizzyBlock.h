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
#ifndef RESOURCE_ELEMENT_SPINDIZZY_BLOCK_H
#define RESOURCE_ELEMENT_SPINDIZZY_BLOCK_H

#include <cmath>

#include <GL/glew.h>
#include <SDL/SDL.h>

#include <IsoRealms/GUI/IComponentCloseListener.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/ElementType/IElement.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>

class SpindizzyBlockConfigurationComponent;

#include "ElementHandlerSpindizzyBlock.h"
#include "ElementSpindizzyBlock.h"
#include "ISpindizzyBlockSet.h"
#include "ISpindizzyBlockType.h"
#include "SpindizzyBlockConfigurationComponent.h"
#include "SpindizzyBlockProperties.h"

class ResourceElementSpindizzyBlock:public ISpindizzyBlockType,
                                    public IComponentCloseListener,
                                    public IResourceUseListener<ITexture> {
  private:
  ISpindizzyBlockSet* cModuleInterface;
    
  std::vector<ElementSpindizzyBlock*> cContent;
  SpindizzyBlockConfigurationComponent* cConfigurationComponent;
  IComponentContainer* cComponentContainer;
  SpindizzyBlockProperties* cBlockProperties;
  ElementSpindizzyBlock* cSampleBlock;
  ElementSpindizzyBlock* cEditingBlock;
  Vertex* cStartLocation;
  BlockTypeProperties cBlockTypeProperties;
  
  unsigned int getXCorner(float);
  
  bool keyDown(SDLKey&, ILayerEditingContext*);  

  ElementSpindizzyBlock* getElement(IElement*);
  
  public:

  /**
   * Construct a new block type of the specified type.  A pointer to comman
   * block properties can be passed for editing and generation of blocks based
   * on the specified type.
   */
  ResourceElementSpindizzyBlock(ISpindizzyBlockSet*, DOMNodeWrapper*, IResourceRegistry*);

  /**
   * Create a new usable block according to the specified parameters.
   * 
   * @param BlockLocation*  Start location of the block.
   * @param BlockLocation*  End location of the block.
   * @param SpindizzyBlockProperties*  Properties to apply to the block.
   * @param bool  true to create an addition, false to create a subtraction.
   * @returns  The new usable block.
   */
  ElementSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, SpindizzyBlockProperties*, bool, ElementHandlerSpindizzyBlock*);
  
  /**********************************\
   * Implements ISpindizzyBlockType *
  \**********************************/
  ISpindizzyBlockSet* getSpindizzyBlockInterface();
  BlockTypeProperties* getBlockTypeProperties();
  void save(DOMNodeWriter*, IResourceLocator*);

  /***************************\
   * Implements IElementType *
  \***************************/
  void configureElement();
  IElement* getElement();
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool, bool);
  void setEditingContext(BlockLocation*, IComponentContainer*);
  bool inputEdit(SDL_Event&, ILayerEditingContext*);
  void renderEditingPreview(Vertex&);
  void updateEditingPreview(unsigned int);
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(IElement*);
  void removeElement(IElement*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  Vertex* editorCursorStopped(Vertex*);

  /**************************************\
   * Implements IComponentCloseListener *
  \**************************************/
  void componentClosed(IHUDComponent*);

  /*******************************\
   * Implements ITextureListener *
  \*******************************/
  void resourcePendingDestruction(ITexture*, ITexture*);
  void resourceChanged(ITexture*);
  
  ~ResourceElementSpindizzyBlock();
};

#endif
