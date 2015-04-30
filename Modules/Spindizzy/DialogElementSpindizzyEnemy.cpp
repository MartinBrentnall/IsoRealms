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

#include "DialogElementSpindizzyEnemy.h"

DialogElementSpindizzyEnemy::DialogElementSpindizzyEnemy(IEditingContext* editingContext, ResourceElementSpindizzyEnemy* elementSpindizzyEnemy, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Enemy Element", resourceName) {
  RectangularComponent* mContent = new RectangularComponent("Modules/Spindizzy/DialogElementSpindizzyEnemy", resources, editingContext, this);
  cElementSpindizzyEnemy = elementSpindizzyEnemy;
  mContent->setValue("resourceModel", cElementSpindizzyEnemy->getModelType());
  addComponent("content", mContent);
}

void DialogElementSpindizzyEnemy::undo() {
  // TODO:
}

ResourceElementSpindizzyEnemy* DialogElementSpindizzyEnemy::getResource() {
  return cElementSpindizzyEnemy;
}

void DialogElementSpindizzyEnemy::valueChanged(const std::string& component, I3DModelType* value) {
  if (component == "resourceModel") {cElementSpindizzyEnemy->setModelType(value);}
}
