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

#include <SFML/Window/Keyboard.hpp>

#include "IsoRealms/Assets/Type/IDigitalInput.h"

namespace IsoRealms {
  class Metadata;
  class IResourceData;

  /**
   * A digital input mapping to a key on a keyboard.
   */
  class KeyboardKey : public IDigitalInput {
    public:
      
    /**
     * Retrieve the key of the specified name.  The key is always such that it
     * may be passed into to the getName() function in order to retrieve the
     * same name as was passed into this function.
     * 
     * @param name Name of the key to retrieve.
     * @return The key of the specified name.
     * @throws ArgumentException If the specified key name is not known.
     */
    static sf::Keyboard::Key getKey(const std::string& name);
    
    /**
     * Retrieve the name of the specified key.  The name is always such that it
     * may be passed into to the getKey() function in order to retrieve the
     * same key as was passed into this function.
     * 
     * @param key The key to retrieve the name of.
     * @return Name of the specified key.
     */
    static std::string getName(const sf::Keyboard::Key& key);
    
    KeyboardKey(const Metadata& metadata, IResourceData& owner);
    KeyboardKey(const Metadata& metadata, IResourceData& owner, JSONObject object);

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
    inline static const std::string JSON_WHICH = "which";

    inline static const std::string UNMAPPED_KEY_PREFIX = "Code ";

    inline static const std::map<std::string, sf::Keyboard::Key> cKeysByName = {
      
      // Function key row.
      {"Escape",       sf::Keyboard::Escape},
      {"F1",           sf::Keyboard::F1},
      {"F2",           sf::Keyboard::F2},
      {"F3",           sf::Keyboard::F3},
      {"F4",           sf::Keyboard::F4},
      {"F5",           sf::Keyboard::F5},
      {"F6",           sf::Keyboard::F6},
      {"F7",           sf::Keyboard::F7},
      {"F8",           sf::Keyboard::F8},
      {"F9",           sf::Keyboard::F9},
      {"F10",          sf::Keyboard::F10},
      {"F11",          sf::Keyboard::F11},
      {"F12",          sf::Keyboard::F12},
      {"F13",          sf::Keyboard::F13},
      {"F14",          sf::Keyboard::F14},
      {"F15",          sf::Keyboard::F15},

      // Number row.
      {"`",            sf::Keyboard::Tilde},
      {"1",            sf::Keyboard::Num1},
      {"2",            sf::Keyboard::Num2},
      {"3",            sf::Keyboard::Num3},
      {"4",            sf::Keyboard::Num4},
      {"5",            sf::Keyboard::Num5},
      {"6",            sf::Keyboard::Num6},
      {"7",            sf::Keyboard::Num7},
      {"8",            sf::Keyboard::Num8},
      {"9",            sf::Keyboard::Num9},
      {"0",            sf::Keyboard::Num0},
      {"[",            sf::Keyboard::LBracket},
      {"]",            sf::Keyboard::RBracket},
      {"Backspace",    sf::Keyboard::BackSpace},

      // Upper letter row.
      {"Tab",          sf::Keyboard::Tab},
      {"'",            sf::Keyboard::Quote},
      {",",            sf::Keyboard::Comma},
      {".",            sf::Keyboard::Period},
      {"P",            sf::Keyboard::P},
      {"Y",            sf::Keyboard::Y},
      {"F",            sf::Keyboard::F},
      {"G",            sf::Keyboard::G},
      {"C",            sf::Keyboard::C},
      {"R",            sf::Keyboard::R},
      {"L",            sf::Keyboard::L},
      {"/",            sf::Keyboard::Slash},
      {"=",            sf::Keyboard::Equal},
      {"\\",           sf::Keyboard::BackSlash},

      // Home letter row.
      {"A",            sf::Keyboard::A},
      {"O",            sf::Keyboard::O},
      {"E",            sf::Keyboard::E},
      {"U",            sf::Keyboard::U},
      {"I",            sf::Keyboard::I},
      {"D",            sf::Keyboard::D},
      {"H",            sf::Keyboard::H},
      {"T",            sf::Keyboard::T},
      {"N",            sf::Keyboard::N},
      {"S",            sf::Keyboard::S},
      {"-",            sf::Keyboard::Dash},
      {"Enter",        sf::Keyboard::Return},

      // Lower letter row.
      {"LeftShift",    sf::Keyboard::LShift},
      {";",            sf::Keyboard::SemiColon},
      {"Q",            sf::Keyboard::Q},
      {"J",            sf::Keyboard::J},
      {"K",            sf::Keyboard::K},
      {"X",            sf::Keyboard::X},
      {"B",            sf::Keyboard::B},
      {"M",            sf::Keyboard::M},
      {"W",            sf::Keyboard::W},
      {"V",            sf::Keyboard::V},
      {"Z",            sf::Keyboard::Z},
      {"RightShift",   sf::Keyboard::RShift},

      // Space bar row.
      {"LeftControl",  sf::Keyboard::LControl},
      {"LeftSuper",    sf::Keyboard::LSystem},
      {"LeftAlt",      sf::Keyboard::LAlt},
      {"Space",        sf::Keyboard::Space},
      {"RightAlt",     sf::Keyboard::RAlt},
      {"Menu",         sf::Keyboard::Menu},
      {"RightSuper",   sf::Keyboard::RSystem},
      {"RightControl", sf::Keyboard::RControl},

      // Misc.
      {"Pause",        sf::Keyboard::Pause},
    
      // Navigation.
      {"Insert",       sf::Keyboard::Insert},
      {"Delete",       sf::Keyboard::Delete},
      {"Home",         sf::Keyboard::Home},
      {"End",          sf::Keyboard::End},
      {"PageUp",       sf::Keyboard::PageUp},
      {"PageDown",     sf::Keyboard::PageDown},
    
      // Arrows.  
      {"ArrowUp",      sf::Keyboard::Up},
      {"ArrowLeft",    sf::Keyboard::Left},
      {"ArrowRight",   sf::Keyboard::Right},
      {"ArrowDown",    sf::Keyboard::Down},
    
      // Keypad.
      {"KPDivide",     sf::Keyboard::Divide},
      {"KPMultiply",   sf::Keyboard::Multiply},
      {"KPMinus",      sf::Keyboard::Subtract},
      {"KPPlus",       sf::Keyboard::Add},
      {"KP0",          sf::Keyboard::Numpad0},
      {"KP1",          sf::Keyboard::Numpad1},
      {"KP2",          sf::Keyboard::Numpad2},
      {"KP3",          sf::Keyboard::Numpad3},
      {"KP4",          sf::Keyboard::Numpad4},
      {"KP5",          sf::Keyboard::Numpad5},
      {"KP6",          sf::Keyboard::Numpad6},
      {"KP7",          sf::Keyboard::Numpad7},
      {"KP8",          sf::Keyboard::Numpad8},
      {"KP9",          sf::Keyboard::Numpad9},
      {"KPEnter",      sf::Keyboard::Enter},
      {"KPPeriod",     sf::Keyboard::Period}
    };

    // External interfaces.
    const Metadata& cMetadata;

    sf::Keyboard::Key cKey; /// The key associated with this mapping.
    // TODO: Support inversion.
  };
}
