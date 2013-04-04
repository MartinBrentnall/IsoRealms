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
#ifndef SPINDIZZY_BLOCK_CONFIGURATION_COMPONENT_H
#define SPINDIZZY_BLOCK_CONFIGURATION_COMPONENT_H

#include <vector>
#include <string>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/GUI/IComponentCloseListener.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/Resources/ElementType/IElement.h>
#include <IsoRealms/ScreenConfiguration.h>

class SpindizzyBlockType;

#include "SpindizzyBlockType.h"
#include "SpindizzyBlockProperties.h"

class SpindizzyBlockConfigurationComponent:public Dialog {
  private:
  SpindizzyBlockProperties* cBlockProperties;
  SpindizzyBlockType* cBlockType;
  IElement* cSampleBlock;

  void updateSampleBlock();

  bool keyDown(SDLKey&);

  public:
  SpindizzyBlockConfigurationComponent(IComponentContainer*, SpindizzyBlockType*, SpindizzyBlockProperties*, IComponentCloseListener*);

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void updateContent(int);
  void renderContent();
  bool inputContent(SDL_Event&);
};

#endif
