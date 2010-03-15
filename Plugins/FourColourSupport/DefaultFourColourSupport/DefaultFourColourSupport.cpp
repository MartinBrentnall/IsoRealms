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
#include "DefaultFourColourSupport.h"

DefaultFourColourSupport::DefaultFourColourSupport() {
  cPalette[FLOOR] = new Colour(0.5, 0.5, 1.0, 0.0);
  cPalette[EXTRA] = new Colour(1.0, 1.0, 1.0, 0.0);
  cPalette[WALL] = new Colour(0.0, 0.0, 0.0, 0.0);
  cPalette[BACKGROUND] = new Colour(0.0, 0.0, 0.5, 0.0);
  cPaletteConfigurationCommand = new PaletteConfigurationCommand(this);
  std::vector<std::string> mPath;
  mPath.push_back("Edit Palette"); // TODO: Use instance name?
  mPath.push_back("Palettes");
  PaletteConfigurationCommandInfo* mPaletteConfigurationCommandInfo = new PaletteConfigurationCommandInfo(mPath, cPaletteConfigurationCommand);
  cPluginCommands.push_back(mPaletteConfigurationCommandInfo);
}

Colour* DefaultFourColourSupport::getColour(PaletteEntry entry) {
  return cPalette[entry];
}

void DefaultFourColourSupport::addChangeListener(IFourColourSupportListener* listener) {
  cChangeListeners.push_back(listener);
}

void DefaultFourColourSupport::removeChangeListener(IFourColourSupportListener* listener) {
  // TODO: Remove listener!
}

std::string DefaultFourColourSupport::getName() {
  return "Custom Four Colour Palette";
}

void DefaultFourColourSupport::setEditingInfo(IComponentContainer* componentContainer) {
  cPaletteConfigurationCommand->setComponentContainer(componentContainer);
}

std::vector<ICommandInfo*> DefaultFourColourSupport::getCommandInfo() {
  return cPluginCommands;
}

DefaultFourColourSupport::PaletteConfigurationCommand::PaletteConfigurationCommand(DefaultFourColourSupport* parent) {
  cParent = parent;
  cComponentContainer = NULL;
}

void DefaultFourColourSupport::PaletteConfigurationCommand::setComponentContainer(IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
}

void DefaultFourColourSupport::PaletteConfigurationCommand::execute() {
  IHUDComponent* mComponent = new PaletteConfigurationComponent(cComponentContainer, cParent->cPalette, cParent->cChangeListeners);
  cComponentContainer->addComponent(mComponent);
}

void DefaultFourColourSupport::save(DOMNodeWriter* node) {
  DOMNodeWriter* mFloorNode = node->addBranch("Floor");
  cPalette[FLOOR]->save(mFloorNode);
  DOMNodeWriter* mExtraNode = node->addBranch("Extra");
  cPalette[EXTRA]->save(mExtraNode);
  DOMNodeWriter* mWallNode = node->addBranch("Wall");
  cPalette[WALL]->save(mWallNode);
  DOMNodeWriter* mBackgroundNode = node->addBranch("Background");
  cPalette[BACKGROUND]->save(mBackgroundNode);
}

void DefaultFourColourSupport::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Floor") {
      cPalette[FLOOR] = new Colour(mNode);
    } else if (mValueAsString == "Extra") {
      cPalette[EXTRA] = new Colour(mNode);
    } else if (mValueAsString == "Wall") {
      cPalette[WALL] = new Colour(mNode);
    } else if (mValueAsString == "Background") {
      cPalette[BACKGROUND] = new Colour(mNode);
    } else {
      // TODO: Throw something!
    }
  }
}

DefaultFourColourSupport::~DefaultFourColourSupport() {
  delete cPaletteConfigurationCommand;
  for (unsigned int i = 0; i < cPluginCommands.size(); i++) {
    delete cPluginCommands[i];
  }
  for (std::map<PaletteEntry, Colour*>::iterator i = cPalette.begin(); i != cPalette.end(); i++) {
    delete i->second;
  }
}

extern "C" IPlugin* create() {
  return new DefaultFourColourSupport();
}

extern "C" void destroy(IPlugin* palette) {
  delete palette;
}
