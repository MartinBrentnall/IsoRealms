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

#include "IsoRealms/Assets/Type/IDigitalInput.h"
#include "IsoRealms/Editing/Property/IOptionalObject.h"

namespace IsoRealms {
  class Metadata;
  class IResourceData;

  /**
   * A digital input mapping to a mouse button.
   */
  class MouseButton : public IDigitalInput {
    public:

    static sf::Mouse::Button getButton(const std::string& name);
    static std::string getName(const sf::Mouse::Button button);

    /**
     * Construct a digital input mapping associated with the specified key.
     *
     * @param key The key to associate this digital input mapping with.
     */
    MouseButton(const sf::Mouse::Button button);

    MouseButton(const Metadata& metadata, IResourceData& owner);
    MouseButton(const Metadata& metadata, IResourceData& owner, JSONObject object);

    /****************************\
     * Implements IDigitalInput *
    \****************************/
    bool matches(const sf::Event& event) const override;
    bool getState(const sf::Event& event) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;

    /***************************************\
     * Implements IAsset via IDigitalInput *
    \***************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    class ButtonChooser : public IOptionalObject {
      public:
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
    };

    inline static const std::string JSON_BUTTON = "button";

    inline static const std::string UNMAPPED_BUTTON_PREFIX = "Mouse Button ";

    inline static const std::map<std::string, sf::Mouse::Button> cButtonsByName = {
      {"Left",         sf::Mouse::Button::Left},
      {"Right",        sf::Mouse::Button::Right},
      {"Middle",       sf::Mouse::Button::Middle},
      {"ExtraButton1", sf::Mouse::Button::XButton1},
      {"ExtraButton2", sf::Mouse::Button::XButton2}
    };

    // External interfaces.
    const Metadata& cMetadata;
    ButtonChooser cButtonChooser;

    sf::Mouse::Button cButton; /// The mouse button associated with this mapping.
    // TODO: Support inversion.
};
}

