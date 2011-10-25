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

Colour* DefaultFourColourSupport::DEFAULT_COLOUR = new Colour(1.0f, 0.0f, 1.0f, 1.0f);

DefaultFourColourSupport::DefaultFourColourSupport() {
  cPaletteConfigurationCommand = new PaletteConfigurationCommand(this);
  std::vector<std::string> mPath;
  mPath.push_back("Edit Palette"); // TODO: Use instance name?
  mPath.push_back("Palettes");
  PaletteConfigurationCommandInfo* mPaletteConfigurationCommandInfo = new PaletteConfigurationCommandInfo(mPath, cPaletteConfigurationCommand);
  cPluginCommands.push_back(mPaletteConfigurationCommandInfo);
}

Colour* DefaultFourColourSupport::getColour(const std::string& entry) {
  std::map<std::string, Colour*>::iterator i = cPalette.find(entry);
  return i == cPalette.end() ? DEFAULT_COLOUR : i->second;
}

void DefaultFourColourSupport::addChangeListener(IFourColourSupportListener* listener) {
  cChangeListeners.push_back(listener);
}

void DefaultFourColourSupport::removeChangeListener(IFourColourSupportListener* listener) {
  // TODO: Remove listener!
}

void DefaultFourColourSupport::setEditingContext(BlockLocation* blockLocation, IComponentContainer* componentContainer) {
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
  for (std::map<std::string, Colour*>::iterator i = cPalette.begin(); i != cPalette.end(); i++) {
    DOMNodeWriter* mColourNode = node->addBranch("Colour");
    mColourNode->addAttribute("name", i->first);
    i->second->save(mColourNode);
  }
}

void DefaultFourColourSupport::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Colour") {
      std::string mColourName = mNode->getAttribute("name");
      cPalette[mColourName] = new Colour(mNode);
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
  for (std::map<std::string, Colour*>::iterator i = cPalette.begin(); i != cPalette.end(); i++) {
    delete i->second;
  }
}

extern "C" IPlugin* create() {
  return new DefaultFourColourSupport();
}

extern "C" void destroy(IPlugin* palette) {
  delete palette;
}
