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
 #include "MenuItemModule.h"

 #include "IsoRealms/Utils.h"
 
 #include "IUIStyle.h"
 
 namespace IsoRealms {
   MenuItemModule::MenuItemModule(const std::string& label, const std::string& tooltip) :
             cLabel(label),
             cTooltip(tooltip) {
   }
 
   float MenuItemModule::getWidth(IUIStyle& style) const {
     IFont* mFont = style.getFont();
     float mFontSize = style.getFontSize();
     return mFont->getWidth(mFontSize, cLabel);
   }
 
   float MenuItemModule::getHeight(IUIStyle& style) const {
     return style.getFontSize() * 2.0f;
   }

   float MenuItemModule::getIndentation(IUIStyle& style) const {
     return 0.0f;
   }
 
   void MenuItemModule::render(IUIStyle& style, float y, float aspectRatio) const {
     IFont* mFont = style.getFont();
     float mFontSize = style.getFontSize();
     mFont->print(-1.0f * aspectRatio, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cLabel);
   }
 
   bool MenuItemModule::input(UISignalID id) {
     return false;
   }
 
   std::string MenuItemModule::getTooltip() const {
     return cTooltip;
   }
 
   bool MenuItemModule::isSelectable() const {
     return true;
   }
 
   std::string MenuItemModule::getLabel() const {
     return cLabel;
   }
 
   bool MenuItemModule::renderIcon() const {
     return false;
   }
 }
 