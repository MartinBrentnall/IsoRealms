/*
 * Copyright 2009 Martin Brentnall
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
#include "OpenCommand.h"

OpenCommand::OpenCommand(SimpleEditor* editor) {
  cEditor = editor;
}

void OpenCommand::execute() {
  std::string mMapName = "Spindizzy Hangworld.isorealms";
  std::string mMapPath = System::getProgramResource("Data/Projects/" + mMapName); // TODO: Should we hard code Data/Projects/ here?
  DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(mMapPath);
  for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Map") {
      Map* mMap = new Map(mNode, cEditor, cEditor, "Spindizzy Hangworld.isorealms", true);
      cEditor->setMap(mMap);
      return;
    }
  }
}
