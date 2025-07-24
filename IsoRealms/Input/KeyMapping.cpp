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
#include "KeyMapping.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/PropertyMaker.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  const std::string KeyMapping::JSON_KEY  = "key";
  const std::string KeyMapping::JSON_TYPE = "type";

  const std::string KeyMapping::TYPE_KEY_DOWN = "KeyDown";

  const std::string KeyMapping::UNMAPPED_KEY_PREFIX = "Code ";

  const std::map<std::string, sf::Keyboard::Key> KeyMapping::cKeysByName = {
    
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

  sf::Keyboard::Key KeyMapping::getKey(const std::string& name) {
    std::map<std::string, sf::Keyboard::Key>::const_iterator i = cKeysByName.find(name);
    if (i == cKeysByName.end()) {
      
      // Handle keys that aren't in the name map.
      if (name.compare(0, UNMAPPED_KEY_PREFIX.length(), UNMAPPED_KEY_PREFIX) == 0) {
        std::stringstream mStream(name); 
        int mCode;
        mStream >> mCode;
        sf::Keyboard::Key mKeyCode = static_cast<sf::Keyboard::Key>(mCode);
        return mKeyCode;
      }
      throw ArgumentException("ERROR: KeyMapping::getKey: Key of name \"" + name + "\" not known.");
    }
    return i->second;
  }

  std::string KeyMapping::getName(const sf::Keyboard::Key& key) {
    for (std::map<std::string, sf::Keyboard::Key>::const_iterator i = cKeysByName.begin(); i != cKeysByName.end(); i++) {
      if (i->second == key) {
        return i->first;
      }
    }
    
    // Handle keys that aren't in the name map.
    return UNMAPPED_KEY_PREFIX + Utils::toString(key);
  }
  
  KeyMapping::KeyMapping(sf::Keyboard::Key key) :
          cKey(key) {
  }
      
  KeyMapping::KeyMapping(JSONObject object) :
          cKey(getKey(object.getString(JSON_KEY))) {
  }

  bool KeyMapping::matches(const sf::Event& event) const {
    return (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && event.key.code == cKey;
  }

  bool KeyMapping::getState(const sf::Event& event) const {
    return event.type == sf::Event::KeyPressed;
  }

  void KeyMapping::save(JSONObject object) const {
    object.addString(JSON_TYPE, TYPE_KEY_DOWN);
    object.addString(JSON_KEY, getShortName());
  }

  void KeyMapping::getProperties(PropertyMaker& owner) {
    owner.createPropertyKey("Key", [this]() {return getShortName();}, [this](sf::Keyboard::Key key) {cKey = key;});
  }

  std::string KeyMapping::getShortName() const {
    return getName(cKey);
  }
    
  std::string KeyMapping::getLongName() const {
    return getName(cKey) + " Key";
  }
}
