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
  cTheme = theme;
  cTable = new ComponentTable(2, 0.02f);
  IResourceSelector* mResourceSelector = editingContext->getResourceSelector();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  
  std::map<SpindizzyZoneThemeTexture*, ITexture*> mTextureElements = theme->getTextureElements();
  cOriginalTextures = mTextureElements;
  for (std::pair<SpindizzyZoneThemeTexture*, ITexture*> mTextureElement : mTextureElements) {
    std::string mElementName = theme->getElementName(mTextureElement.first);
    TextLabelComponent* mTextureName = new TextLabelComponent(mElementName + ": ");
    ComponentResourceTextureSelector* mTextureSelector = new ComponentResourceTextureSelector(this, mTextureElement.second, mResourceSelector);
    cTextureSelectorElementMap[mTextureSelector] = mTextureElement.first;
    SelectableComponent* mTextureSelectable = new SelectableComponent(mTextureSelector, 0.1f * mAspectRatio, 0.1f);
    std::vector<ISizedComponent*> mRow;
    mRow.push_back(mTextureName);
    mRow.push_back(mTextureSelectable);
    cTable->addRow(mRow);
  }

  std::map<SpindizzyZoneThemeColour*, IColour*> mColourElements = theme->getColourElements();
  cOriginalColours = mColourElements;
  for (std::pair<SpindizzyZoneThemeColour*, IColour*> mColourElement : mColourElements) {
    std::string mElementName = theme->getElementName(mColourElement.first);
    TextLabelComponent* mColourName = new TextLabelComponent(mElementName + ": ");
    ComponentResourceColourSelector* mColourSelector = new ComponentResourceColourSelector(this, mColourElement.second, mResourceSelector);
    cColourSelectorElementMap[mColourSelector] = mColourElement.first;
    SelectableComponent* mColourSelectable = new SelectableComponent(mColourSelector, 0.1f * mAspectRatio, 0.1f);
    std::vector<ISizedComponent*> mRow;
    mRow.push_back(mColourName);
    mRow.push_back(mColourSelectable);
    cTable->addRow(mRow);
  }
  
  addComponent("content", cTable);
}

void DialogSpindizzyZoneTheme::undo() {
  for (std::pair<SpindizzyZoneThemeTexture*, ITexture*> mOriginal : cOriginalTextures) {
    cTheme->registerElement(mOriginal.first, mOriginal.second);
  }
  for (std::pair<SpindizzyZoneThemeColour*, IColour*> mOriginal : cOriginalColours) {
    cTheme->registerElement(mOriginal.first, mOriginal.second);
  }
}

void DialogSpindizzyZoneTheme::selected(ISelector* selector, IColour* colour) {
  SpindizzyZoneThemeColour* mElement = cColourSelectorElementMap[selector];
  cTheme->registerElement(mElement, colour);
}

void DialogSpindizzyZoneTheme::selected(ISelector* selector, ITexture* texture) {
  SpindizzyZoneThemeTexture* mElement = cTextureSelectorElementMap[selector];
  cTheme->registerElement(mElement, texture);
}
