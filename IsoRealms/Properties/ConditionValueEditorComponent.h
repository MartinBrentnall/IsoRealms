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
#ifndef CONDITION_VALUE_EDITOR_COMPONENT_H
#define CONDITION_VALUE_EDITOR_COMPONENT_H

#include "../Configuration.h"
#include "../ScreenConfiguration.h"

#include <IsoRealms/GUI/IComponentBoundsCalculator.h>
#include <IsoRealms/GUI/IComponentSizeCalculator.h>
#include <IsoRealms/GUI/ISizedComponent.h>
#include <IsoRealms/IConditionElementIcons.h>

#include "ConditionComponent.h"

class ConditionValueEditorComponent:public ConditionComponent {
  public:
  ConditionValueEditorComponent(Condition*, IConditionElementIcons*);

  /******************************\
   * Implements ISizedComponent *
  \******************************/
  bool input(SDL_Event&);
  
  virtual ~ConditionValueEditorComponent() {};
};

#endif
