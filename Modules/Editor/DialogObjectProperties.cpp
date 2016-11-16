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
#include "DialogObjectProperties.h"

DialogObjectProperties::DialogObjectProperties(IComponentContainer* container, IResourceAccessor* resources) : Dialog(container, "Modules/Editor/DialogObjectProperties", resources) {
  cPropertiesTable = new ComponentTable(2);
  addComponent("properties", cPropertiesTable);
  setTitle("Properties (no selection)");
}

void DialogObjectProperties::objectSelected(IObjectWithProperties* object) {
  cPropertiesTable->clear();
  if (object != nullptr) {
    std::string mObjectTypeName = object->getTypeName();
    setTitle("Properties of " + mObjectTypeName);
    std::vector<IObjectProperty*> mObjectProperties = object->getProperties();
    for (IObjectProperty* mObjectProperty : mObjectProperties) {
      std::string mPropertyName = mObjectProperty->getPropertyName();
      TextLabelComponent* mPropertyNameLabel = new TextLabelComponent(mPropertyName);
      TextFieldComponent* mPropertyNameValue = new TextFieldComponent("1000");
      std::vector<ISizedComponent*> mRow;
      mRow.push_back(mPropertyNameLabel);
      mRow.push_back(mPropertyNameValue);
      cPropertiesTable->addRow(mRow);
    }
  } else {
    setTitle("Properties (no selection)");
  }
}
