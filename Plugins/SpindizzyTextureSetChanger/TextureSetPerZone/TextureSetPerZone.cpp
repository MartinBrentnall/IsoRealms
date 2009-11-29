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
#include "TextureSetPerZone.h"

TextureSetPerZone::TextureSetPerZone() {
  std::vector<std::string> mPath;
  mPath.push_back("Choose Texture");
  // TODO: How to identify which element set(s) are being controlled by this plugin?
  mPath.push_back("Zone");
  cChooseTextureSetCommand = new ChooseTextureSetCommand(this);
  ChooseTextureSetCommandInfo* mChooseTextureSetCommandInfo = new ChooseTextureSetCommandInfo(mPath, cChooseTextureSetCommand);
  cPluginCommands.push_back(mChooseTextureSetCommandInfo);
}

void TextureSetPerZone::setControlObject(IChangeableTextureSet* objectToControl) {
  cControlledObject = objectToControl;
}

std::string TextureSetPerZone::getName() {
  return "Texture Set Per Zone";
}

std::vector<PlugSocket*> TextureSetPerZone::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  // TODO: Should not construct plug sockets on-the-fly.
  for (unsigned int i = 0; i <= cTexturePalette.size(); i++) {
    std::ostringstream mSocketID;
    mSocketID << i;
    PlugSocket* mPlugSocket = new PlugSocket("SpindizzyTextureSet", mSocketID.str());
    mSockets.push_back(mPlugSocket);
  }
  return mSockets;
}

void TextureSetPerZone::setPlugin(PlugSocket* plugSocket, IPlugin* plugin) {
  if (plugSocket->getType() == "SpindizzyTextureSet") {
    std::string mSocketID = plugSocket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    if (plugin != NULL) {
      ISpindizzyTextureSet* mTextureSet = dynamic_cast<ISpindizzyTextureSet*>(plugin);
      if (mTextureSet != NULL) {
        if (mIndex == cTexturePalette.size()) {
          cTexturePalette.push_back(mTextureSet);
        } else {
          cTexturePalette[mIndex] = mTextureSet;
        }
      } else {
        std::cout << "Warning: dynamic_cast failed for ISpindizzyTextureSet" << std::endl;
      }
    } else if (mIndex != cTexturePalette.size()) {
      cTexturePalette.erase(cTexturePalette.begin() + mIndex);
    }
  }
}

IPlugin* TextureSetPerZone::getPlugin(PlugSocket* plugSocket) {
  if (plugSocket->getType() == "SpindizzyTextureSet") {
    std::string mSocketID = plugSocket->getID();
    std::stringstream mInputString(mSocketID);
    int mIndex;
    mInputString >> mIndex;
    return cTexturePalette[mIndex]; 
  }
  return NULL;
}

void TextureSetPerZone::notifyZoneAction(Zone* zone) {
  // Nothing to do.
}

void TextureSetPerZone::initPlugin(Zone* zone) {
  // Nothing to do.
}

std::vector<ICommandInfo*> TextureSetPerZone::getCommandInfo() {
  return cPluginCommands;
}

void TextureSetPerZone::setEditingInfo(IComponentContainer* componentContainer) {
  cChooseTextureSetCommand->setComponentContainer(componentContainer);
}

void TextureSetPerZone::save(DOMNodeWriter*) {
  // Nothing to do.
}

void TextureSetPerZone::load(DOMNodeWrapper*) {
  // Nothing to do.
}

TextureSetPerZone::ChooseTextureSetCommand::ChooseTextureSetCommand(TextureSetPerZone* parent) {
  cParent = parent;
  cComponentContainer = NULL;
}

void TextureSetPerZone::ChooseTextureSetCommand::setComponentContainer(IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
}

void TextureSetPerZone::ChooseTextureSetCommand::execute() {
  IHUDComponent* mComponent = new TextureSetChooserComponent(cComponentContainer, cParent->cTexturePalette/*, cParent->cChangeListeners*/);
  cComponentContainer->addComponent(mComponent);
}

extern "C" IPlugin* create() {
  return new TextureSetPerZone();
}

extern "C" void destroy(IPlugin* textureSetPerZone) {
  delete textureSetPerZone;
}

