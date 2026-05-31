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

#include <map>

#include <SFML/Window/Mouse.hpp>

#include "IsoRealms/Assets/Type/IDigitalInputMapping.h"

namespace IsoRealms {
  class Metadata;
  class IResourceData;

  /**
   * A digital input mapping to a mouse button.
   */
  class MouseButtonMapping : public IDigitalInputMapping {
    private:
    inline static const std::string JSON_BUTTON = "button";

    inline static const std::string UNMAPPED_BUTTON_PREFIX = "Mouse Button ";

    inline static const std::map<std::string, sf::Mouse::Button> cButtonsByName = {
      {"Left",         sf::Mouse::Button::Left},
      {"Right",        sf::Mouse::Button::Right},
      {"Middle",       sf::Mouse::Button::Middle},
      {"ExtraButton1", sf::Mouse::Button::XButton1},
      {"ExtraButton2", sf::Mouse::Button::XButton2}
    };

    sf::Mouse::Button cButton; /// The mouse button associated with this mapping.
    // TODO: Support inversion.

    public:

    static sf::Mouse::Button getButton(const std::string& name);
    static std::string getName(const sf::Mouse::Button button);

    /**
     * Construct a digital input mapping associated with the specified key.
     *
     * @param key The key to associate this digital input mapping with.
     */
    MouseButtonMapping(const sf::Mouse::Button button);

    MouseButtonMapping(const Metadata& metadata, IResourceData& owner);
    MouseButtonMapping(const Metadata& metadata, IResourceData& owner, JSONObject object);

    /***********************************\
     * Implements IDigitalInputMapping *
    \***********************************/
    bool matches(const sf::Event& event) const override;
    bool getState(const sf::Event& event) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;

    /**********************************************\
     * Implements IAsset via IDigitalInputMapping *
    \**********************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;
  };
}

