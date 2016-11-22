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
#include "DialogSpindizzyZoneTheme.h"

DialogSpindizzyZoneTheme::DialogSpindizzyZoneTheme(IEditingContext* editingContext, IResourceAccessor* resources, ISpindizzyZoneTheme* theme) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Zone Theme", theme->getName()) {
  cTable = new ComponentTable(2, 0.02f);
  IResourceSelector* mResourceSelector = editingContext->getResourceSelector();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  
  std::map<std::string, ITexture*> mTextureElements = theme->getTextureElements();
  for (std::pair<std::string, ITexture*> mTextureElement : mTextureElements) {
    TextLabelComponent* mTextureName = new TextLabelComponent(mTextureElement.first + ": ");
    ComponentResourceTextureSelector* mTextureSelector = new ComponentResourceTextureSelector(this, mTextureElement.second, mResourceSelector);
    SelectableComponent* mTextureSelectable = new SelectableComponent(mTextureSelector, 0.1f * mAspectRatio, 0.1f);
    std::vector<ISizedComponent*> mRow;
    mRow.push_back(mTextureName);
    mRow.push_back(mTextureSelectable);
    cTable->addRow(mRow);
  }

  std::map<std::string, IColour*> mColourElements = theme->getColourElements();
  for (std::pair<std::string, IColour*> mColourElement : mColourElements) {
    TextLabelComponent* mColourName = new TextLabelComponent(mColourElement.first);
    ComponentResourceColourSelector* mColourSelector = new ComponentResourceColourSelector(this, mColourElement.second, mResourceSelector);
    SelectableComponent* mColourSelectable = new SelectableComponent(mColourSelector, 0.1f * mAspectRatio, 0.1f);
    std::vector<ISizedComponent*> mRow;
    mRow.push_back(mColourName);
    mRow.push_back(mColourSelectable);
    cTable->addRow(mRow);
  }
  
  addComponent("content", cTable);
}

void DialogSpindizzyZoneTheme::undo() {
  // TODO: Implement this
}

void DialogSpindizzyZoneTheme::selected(ISelector*, IColour*) {
  // TODO: Implement this
}

void DialogSpindizzyZoneTheme::selected(ISelector*, ITexture*) {
  // TODO: Implement this
}
