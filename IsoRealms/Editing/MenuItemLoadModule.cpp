/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
 #include "MenuItemLoadModule.h"

 #include "IsoRealms/Assets/Type/IFont.h"
 
 #include "IUIStyle.h"
 
 namespace IsoRealms {
   MenuItemLoadModule::MenuItemLoadModule(IPropertyManager& manager, std::unique_ptr<IProperty> property) :
             cManager(manager),
             cProperty(std::move(property)) {
   }
 
   IProperty* MenuItemLoadModule::getProperty() const {
     return cProperty.get();
   }
 
   float MenuItemLoadModule::getValueWidth(IUIStyle& style) const {
     return cProperty->getValueWidth(style);
   }
   
   float MenuItemLoadModule::getWidth(IUIStyle& style) const {
     return cProperty->getValueWidth(style);
   }
 
   float MenuItemLoadModule::getHeight(IUIStyle& style) const {
     return style.getFontSize() * 2.0f;
   }
 
   float MenuItemLoadModule::getIndentation(IUIStyle& style) const {
     return 0.0f;
   }
   
   void MenuItemLoadModule::render(IUIStyle& style, float y, float aspectRatio) const {
     cProperty->renderValue(style, y, -1.0f * aspectRatio, aspectRatio);
   }
 
   bool MenuItemLoadModule::input(UISignalID id, float y) {
     if (id == UISignalID::CONFIRM) {
      cProperty->confirm(cManager, y);
      return true;
     }
     return false;
   }
 
   std::string MenuItemLoadModule::getTooltip() const {
     return cProperty->getTooltip();
   }
 
   bool MenuItemLoadModule::isSelectable() const {
     return true;
   }
 }
 