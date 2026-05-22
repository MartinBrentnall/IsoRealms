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
 #pragma once

 #include <functional>
 #include <memory>
 #include <string>
 
 #include "Property/IProperty.h"
 
 #include "IMenuItem.h"
 
 namespace IsoRealms {
   class MenuItemLoadModule : public IMenuItem {
     public:
     MenuItemLoadModule(std::unique_ptr<IProperty> property);
 
     IProperty* getProperty() const;
     float getValueWidth(IUIStyle& style) const;
 
     /************************\
      * Implements IMenuItem *
     \************************/
     float getWidth(IUIStyle& style) const override;
     float getHeight(IUIStyle& style) const override;
     float getIndentation(IUIStyle& style) const override;
     void render(IUIStyle& style, float y, float aspectRatio) const override;
     bool input(UISignalID id) override;
     std::string getTooltip() const override;
     bool isSelectable() const override;
 
     private:
     std::unique_ptr<IProperty> cProperty;
   };
 }
 