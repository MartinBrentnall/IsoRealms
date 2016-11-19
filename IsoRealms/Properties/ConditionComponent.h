/*
 * Copyright 2016 Martin Brentnall
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
#ifndef CONDITION_COMPONENT_H
#define CONDITION_COMPONENT_H

#include "../Configuration.h"
#include "../ScreenConfiguration.h"

#include <IsoRealms/Condition.h>
#include <IsoRealms/GUI/IComponentBoundsCalculator.h>
#include <IsoRealms/GUI/IComponentSizeCalculator.h>
#include <IsoRealms/GUI/ISizedComponent.h>
#include <IsoRealms/GUI/TextLabelComponent.h>
#include <IsoRealms/IConditionElementIcons.h>
#include <IsoRealms/MultipleClickDetector.h>

#include "ConditionElementIcon.h"

class ConditionComponent:public ISizedComponent {
  private:
  std::vector<ISizedComponent*> cComponents;
  MultipleClickDetector cMultipleClickDetector;
  IComponentContainer* cComponentContainer;
  Condition* cCondition;
  IConditionElementIcons* cConditionElementIcons;
  float cIconSize;

  float getComponentWidth(unsigned int);

  class CellLayout:public IComponentBoundsCalculator {
    private:
    ConditionComponent* cParent;
    unsigned int cLocation;

    public:
    CellLayout(ConditionComponent*, unsigned int);

    /*****************************************\
     * Implements IComponentBoundsCalculator *
    \*****************************************/
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };

  std::vector<ISizedComponent*> getConditionIcons(Condition*);

  public:
  ConditionComponent(Condition*, IConditionElementIcons*, float);

  Condition* getCondition();
  IConditionElementIcons* getConditionElementIcons();
  
  /******************************\
   * Implements ISizedComponent *
  \******************************/
  float getWidth();
  float getHeight();
  void update(unsigned int);
  void render();
  
  virtual ~ConditionComponent() {};
};

#endif

