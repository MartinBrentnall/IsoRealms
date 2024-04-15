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
#include "InputHandler.h"

namespace IsoRealms {
  InputHandler::InputHandler(IProject* project) :
            cProject(project),
            cInputHandler(cProject->createLiteralInputHandler(this)) {
  }

  void InputHandler::init(DOMNode& node, const std::string& tag) {
    DOMNode& mAssetNode = node.getNode(tag);
    cProject->init([this, &mAssetNode](IAssets* assets) {
      cProject->release(this, cInputHandler);
      cInputHandler = cProject->getInputHandler(this, mAssetNode);
    });
  }

  void InputHandler::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mAssetNode = node->addBranch(tag);
    cProject->save(&mAssetNode, cInputHandler);
  }

  void InputHandler::relinquish(IInputHandler* asset) {
    if (cInputHandler == asset) {
      cInputHandler = cProject->createLiteralInputHandler(this);
    }
  }

  InputHandler::~InputHandler() {
    if (cInputHandler != nullptr) {
      cProject->release(this, cInputHandler);
    }
  }
}

