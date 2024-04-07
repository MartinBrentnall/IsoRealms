/*
 * Copyright 2023 Martin Brentnall
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
#include "InputGroup.h"

namespace IsoRealms::Basics {
  const std::string InputGroup::TAG_ELEMENT       = "Element";
  const std::string InputGroup::TAG_INPUT_HANDLER = "InputHandler";

  const std::string InputGroup::PROPERTY_INPUT_HANDLER = "Input";
  
  InputGroup::InputGroup(IProject* project, Basics* basics) {
  }
  
  InputGroup::InputGroup(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            InputGroup(project, basics) {
    for (DOMNode& mChild : node) {
      std::string mName = mChild.getName();
      if (mName == TAG_ELEMENT) {
        cDefInputHandlers.emplace_back(std::make_unique<InputHandler>(project));
        cDefInputHandlers.back()->init(mChild, TAG_INPUT_HANDLER);
      } else {
        throw ParseException("Unknown tag for Basics/InputGroup: " + mName);
      }
    }
  }

  void InputGroup::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Input Groups");
  }

  void InputGroup::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }

  void InputGroup::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    for (const std::unique_ptr<InputHandler>& mInputHandler : cDefInputHandlers) {
      DOMNodeWriter mElementNode = node->addBranch(TAG_ELEMENT);
      mInputHandler->save(&mElementNode, TAG_INPUT_HANDLER);
    }
  }

  void InputGroup::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool InputGroup::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> InputGroup::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  bool InputGroup::input(sf::Event& event) {
    for (std::unique_ptr<InputHandler>& mInputHandler : cDefInputHandlers) {
      if ((*mInputHandler)->input(event)) {
        return true;
      }
    }
    return false;
  }

  bool InputGroup::renderAssetIcon() const {
    return renderIcon();
  }
}
