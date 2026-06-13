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

#include <limits>
#include <functional>
#include <string>

#include <SFML/Window/Event.hpp>

#include "IPropertyEditor.h"
#include "Property.h"

namespace IsoRealms {
  class PropertyKey : public Property {
    public:
    PropertyKey(const PropertyData& data, IComponentAccessManager& resourceAccessManager, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction = nullptr);
      
    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override;
    float getValueWidth(IUIStyle& style) const override;
    void confirm(IPropertyManager& manager, float y) override;
    bool hasConfiguration() const override;
    void configure(IPropertyManager& manager) override;

    private:
    class Editor : public IPropertyEditor {
      public:
      Editor(PropertyKey& parent);

      /******************************\
       * Implements IPropertyEditor *
      \******************************/
      void render(IUIStyle& style, float y, float x, float aspectRatio) const override;
      bool update(unsigned int milliseconds) override;
      bool input(UISignalID id, IUIStyle& style) override;
      bool input(sf::Event& event, IUIStyle& style) override;

      private:
      PropertyKey& cParent;
    };
    
    std::function<std::string()> cGetter;
    std::function<void(sf::Keyboard::Key)> cSetter;
  };
}
