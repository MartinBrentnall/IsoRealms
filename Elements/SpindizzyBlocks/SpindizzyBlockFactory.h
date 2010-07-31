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
#ifndef SPINDIZZY_BLOCK_FACTORY_H
#define SPINDIZZY_BLOCK_FACTORY_H

#include <GL/gl.h>
#include <SDL/SDL.h>

#include <IsoRealms/GUI/IComponentCloseListener.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IElement.h>
#include <IsoRealms/IElementFactory.h>
#include <IsoRealms/IElementGateway.h>
#include <IsoRealms/IElementSet.h>
#include <IsoRealms/IsoRealmsConstants.h>

#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"

class SpindizzyBlockConfigurationComponent;

#include "AbstractSpindizzyBlock.h"
#include "ISpindizzyBlockFactory.h"
#include "ISpindizzyBlockSet.h"
#include "SpindizzyBlockConfigurationComponent.h"
#include "SpindizzyBlockProperties.h"

class SpindizzyBlockFactory:public ISpindizzyBlockFactory,
                            public IComponentCloseListener {
  private:
  std::string cFactoryName;
  std::vector<AbstractSpindizzyBlock*> cContent;
  SpindizzyBlockConfigurationComponent* cConfigurationComponent;
  IComponentContainer* cComponentContainer;
  ISpindizzyTextureSet** cSpindizzyTextureSet;
  SpindizzyBlockProperties* cBlockProperties;
  BlockLocation* cStartBlockLocation;
  BlockLocation* cEditingLocation;
  AbstractSpindizzyBlock* cSampleBlock;
  Script* cContactScript;
  float cSurfaceFriction;
  float cSurfaceGrip;
  bool cRespawnAllowed;

  bool keyDown(SDLKey&);  

  void drawSlopeWireFrameCuboid(BlockLocation* location, BlockLocation* dimension, int xslope, int yslope);

  public:

  /**
   * Construct a new block factory of the specified type.  A pointer to comman
   * block properties can be passed for editing and generation of blocks based
   * on the specified type.
   * 
   * @param ISpindizzyTextureSet*  A texture set to use for texturing blocks.
   */
  SpindizzyBlockFactory(std::string, ISpindizzyTextureSet**, ISpindizzyBlockSet*);

  /**
   * Create a new usable block according to the specified parameters.
   * 
   * @param BlockLocation*  Start location of the block.
   * @param BlockLocation*  End location of the block.
   * @param ISpindizzyTextureSet*  Textures to use for the block.
   * @param SpindizzyBlockProperties*  Properties to apply to the block.
   * @param bool  true to create an addition, false to create a subtraction.
   * @returns  The new usable block.
   */
  virtual AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool) = 0;
  
  /*************************************\
   * Implements ISpindizzyBlockFactory *
  \*************************************/
  void signalAllElementsDirty();
  void unregisterSurfaces(ISurfaceProcessor*);
  Script* getContactScript();
  float getSurfaceFriction();
  float getSurfaceGrip();
  bool isRespawnAllowed();
  void configureBlock(DOMNodeWrapper*, ICommandRegistry*);

  /******************************\
   * Implements IElementFactory *
  \******************************/
  void configureElement();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  void setEditingContext(BlockLocation*, IComponentContainer*);
  bool input(SDL_Event&);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(int);
  std::string getName();

  /**************************************\
   * Implements IComponentCloseListener *
  \**************************************/
  void componentClosed(IHUDComponent*);

  ~SpindizzyBlockFactory();
};

#endif
