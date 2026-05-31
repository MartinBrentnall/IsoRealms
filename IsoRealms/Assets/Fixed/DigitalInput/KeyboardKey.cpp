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
#include "KeyboardKey.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/PropertyMaker.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  sf::Keyboard::Key KeyboardKey::getKey(const std::string& name) {
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

  std::string KeyboardKey::getName(const sf::Keyboard::Key& key) {
    for (std::map<std::string, sf::Keyboard::Key>::const_iterator i = cKeysByName.begin(); i != cKeysByName.end(); i++) {
      if (i->second == key) {
        return i->first;
      }
    }
    
    // Handle keys that aren't in the name map.
    return UNMAPPED_KEY_PREFIX + Utils::toString(key);
  }
  
  KeyboardKey::KeyboardKey(const Metadata& metadata, IResourceData& owner) :
          cMetadata(metadata) {
    // TODO: Implement this.
  }

  KeyboardKey::KeyboardKey(const Metadata& metadata, IResourceData& owner, JSONObject object) :
          KeyboardKey(metadata, owner) {
    cKey = getKey(object.getString(JSON_WHICH));
  }

  bool KeyboardKey::matches(const sf::Event& event) const {
    return (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && event.key.code == cKey;
  }

  bool KeyboardKey::getState(const sf::Event& event) const {
    return event.type == sf::Event::KeyPressed;
  }

  std::string KeyboardKey::getShortName() const {
    return getName(cKey);
  }
    
  std::string KeyboardKey::getLongName() const {
    return getName(cKey) + " Key";
  }

  bool KeyboardKey::renderAssetIcon() const {
    return false;
  }

  void KeyboardKey::saveAsset(JSONObject object) const {
    object.addString(JSON_WHICH, getShortName());
  }

  void KeyboardKey::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyKey(cMetadata.getPropertyData("Key"), [this]() {return getShortName();}, [this](sf::Keyboard::Key key) {cKey = key;});
  }

  bool KeyboardKey::isDefaultConfiguration() const {
    return true;
  }
}
