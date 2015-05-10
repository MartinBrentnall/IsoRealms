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

#include "DialogIntegerTimer.h"

DialogIntegerTimer::DialogIntegerTimer(IEditingContext* editingContext, ResourceIntegerTimer* integer, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Timer", resourceName) {
  cContent = new RectangularComponent("Modules/IsoRealms/DialogIntegerTimer", resources);
  cInteger = integer;
  cOriginalValue = cInteger->getInitialValue();
  cContent->addIntegerListener(this, "integerFieldValue");
  cContent->setIntegerValue("integerFieldValue", cOriginalValue);
  addComponent("content", cContent);
}

void DialogIntegerTimer::undo() {
  cInteger->setInitialValue(cOriginalValue);
  cContent->setIntegerValue("integerFieldValue", cOriginalValue);
}

ResourceIntegerTimer* DialogIntegerTimer::getResource() {
  return cInteger;
}

void DialogIntegerTimer::valueChanged(IValueComponent<int>* component, int value) {
  cInteger->setInitialValue(value);
}
