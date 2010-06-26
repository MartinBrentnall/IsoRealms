/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef SPINDIZZY_LIFT_CONFIGURATION_COMPONENT_H
#define SPINDIZZY_LIFT_CONFIGURATION_COMPONENT_H

#include <string>

#include <IsoRealms/GUI/Button.h>
#include <IsoRealms/GUI/CloseDialogCommand.h>
#include <IsoRealms/GUI/ComponentEdgeLayout.h>
#include <IsoRealms/GUI/EdgeRelation.h>
#include <IsoRealms/GUI/FlexibleGridLayoutComponent.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/ScrollableContainer.h>
#include <IsoRealms/GUI/TextLabelComponent.h>
#include <IsoRealms/GUI/TextFieldComponent.h>
#include <IsoRealms/IComponentContainer.h>

#include "SpindizzyLiftProperties.h"

class SpindizzyLiftConfigurationComponent:public ResizableDialog {
  private:
  SpindizzyLiftProperties* cProperties;
  TextFieldComponent* cUpSpeedField;
  TextFieldComponent* cDownSpeedField;
  TextFieldComponent* cTopDelayField;
  TextFieldComponent* cBottomDelayField;
  
  std::string toString(unsigned int);

  class ApplyCommand:public ICommand {
    private:
    SpindizzyLiftConfigurationComponent* cParent;

    public:
    ApplyCommand(SpindizzyLiftConfigurationComponent*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:
  SpindizzyLiftConfigurationComponent(IComponentContainer*, SpindizzyLiftProperties*);

  /******************************\
   * Implements ResizableDialog *
  \******************************/
  bool inputResizableDialogContent(SDL_Event&);
  void renderResizableDialogContent();
  void updateResizableDialogContent(int ticks);
};

#endif
