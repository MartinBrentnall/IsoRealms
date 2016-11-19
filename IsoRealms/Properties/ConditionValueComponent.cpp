
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
#include "ConditionValueComponent.h"

ConditionValueComponent::ConditionValueComponent(Condition* condition, IConditionElementIcons* icons, IComponentContainer* componentContainer) : ConditionComponent(condition, icons, 0.05f) {
  cComponentContainer = componentContainer;
}

bool ConditionValueComponent::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
      float mX = mScreen->getXLocation(event.button.x);
      float mY = mScreen->getYLocation(event.button.y);
      if (contains(mX, mY)) {
        cMultipleClickDetector.input(event);
        if (cMultipleClickDetector.getClicks() == MultipleClickDetector::DOUBLE_CLICK && event.button.button == SDL_BUTTON_LEFT) {
          cEditorDialog = new DialogCondition(cComponentContainer, nullptr, getCondition(), getConditionElementIcons());
          cComponentContainer->addComponent(cEditorDialog);
        }
        return true;
      }
    }
  }
  return false;
}
