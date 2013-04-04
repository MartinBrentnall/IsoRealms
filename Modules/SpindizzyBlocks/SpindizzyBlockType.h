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
#ifndef SPINDIZZY_BLOCK_TYPE_H
#define SPINDIZZY_BLOCK_TYPE_H

#include <GL/glew.h>
#include <SDL/SDL.h>

#include <IsoRealms/GUI/IComponentCloseListener.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/ElementType/IElement.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>

class SpindizzyBlockConfigurationComponent;

#include "ISpindizzyBlockSet.h"
#include "ISpindizzyBlockType.h"
#include "SpindizzyBlock.h"
#include "SpindizzyBlockConfigurationComponent.h"
#include "SpindizzyBlockProperties.h"

class SpindizzyBlockType:public ISpindizzyBlockType,
                         public IComponentCloseListener,
			 public IResourceUseListener<ITexture> {
  private:
  std::vector<SpindizzyBlock*> cContent;
  SpindizzyBlockConfigurationComponent* cConfigurationComponent;
  IComponentContainer* cComponentContainer;
  SpindizzyBlockProperties* cBlockProperties;
  BlockLocation* cStartBlockLocation;
  BlockLocation* cEditingLocation;
  SpindizzyBlock* cSampleBlock;
  BlockTypeProperties cBlockTypeProperties;
  
  bool keyDown(SDLKey&);  

  void drawSlopeWireFrameCuboid(BlockLocation* location, BlockLocation* dimension, int xslope, int yslope);

  public:

  /**
   * Construct a new block type of the specified type.  A pointer to comman
   * block properties can be passed for editing and generation of blocks based
   * on the specified type.
   */
  SpindizzyBlockType(ISpindizzyBlockSet*);

  /**
   * Create a new usable block according to the specified parameters.
   * 
   * @param BlockLocation*  Start location of the block.
   * @param BlockLocation*  End location of the block.
   * @param SpindizzyBlockProperties*  Properties to apply to the block.
   * @param bool  true to create an addition, false to create a subtraction.
   * @returns  The new usable block.
   */
  SpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, SpindizzyBlockProperties*, bool);
  
  /**********************************\
   * Implements ISpindizzyBlockType *
  \**********************************/
  void unregisterSurfaces(ISurfaceProcessor*);
  BlockTypeProperties* getBlockTypeProperties();
  void save(DOMNodeWriter*, IResourceLocator*);

  /***************************\
   * Implements IElementType *
  \***************************/
  void configureElement();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  IElementHandler* getElementHandler();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  bool input(SDL_Event&);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(int);
  std::string getName();
  void destroy(IElement*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  /**************************************\
   * Implements IComponentCloseListener *
  \**************************************/
  void componentClosed(IHUDComponent*);

  /*******************************\
   * Implements ITextureListener *
  \*******************************/
  void resourcePendingDestruction(ITexture*, ITexture*);
  void resourceChanged(ITexture*);
  
  ~SpindizzyBlockType();
};

#endif
