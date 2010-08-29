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

#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/GUI/IComponentCloseListener.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IElement.h>
#include <IsoRealms/ScreenConfiguration.h>

class SpindizzyBlockFactory;

#include "SpindizzyBlockFactory.h"
#include "SpindizzyBlockProperties.h"

class SpindizzyBlockConfigurationComponent:public Dialog {
  private:
  ISpindizzyTextureSet** cTextureSet;
  SpindizzyBlockProperties* cBlockProperties;
  SpindizzyBlockFactory* cBlockFactory;
  IElement* cSampleBlock;

  void updateSampleBlock();

  bool keyDown(SDLKey&);

  public:
  SpindizzyBlockConfigurationComponent(IComponentContainer*, SpindizzyBlockFactory*, SpindizzyBlockProperties*, ISpindizzyTextureSet**, IComponentCloseListener*);

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void updateContent(int);
  void renderContent();
  bool inputContent(SDL_Event&);
};

#endif
