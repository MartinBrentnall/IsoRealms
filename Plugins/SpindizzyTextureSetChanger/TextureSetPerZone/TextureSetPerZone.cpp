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
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  std::vector<std::string> mPath;
  mPath.push_back("Choose Texture");
  // TODO: How to identify which element set(s) are being controlled by this plugin?
  mPath.push_back("Zone");
  cChooseTextureSetCommand = new ChooseTextureSetCommand(this);
  ChooseTextureSetCommandInfo* mChooseTextureSetCommandInfo = new ChooseTextureSetCommandInfo(mPath, cChooseTextureSetCommand);
  cPluginCommands.push_back(mChooseTextureSetCommandInfo);
  cProgressBackgroundColour = 1.0f;
}

void TextureSetPerZone::setTextureSet(ISpindizzyTextureSet* textureSet) {
  if (textureSet != NULL) {
    cZoneMapping[cCurrentZone] = textureSet;
  } else {
    cZoneMapping.erase(cCurrentZone);
  }
  cCurrentMap->zoneChanged(cCurrentZone);
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
  mSockets.push_back(new PlugSocket("ZoneContext"));
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
  } else if (plugSocket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *plugSocket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  }
}

IPlugin* TextureSetPerZone::getPlugin(PlugSocket* plugSocket) {
  if (plugSocket->getType() == "SpindizzyTextureSet") {
    std::string mSocketID = plugSocket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    if (mIndex < cTexturePalette.size()) {
      return cTexturePalette[mIndex]; 
    }
  } else if (plugSocket->getType() == "ZoneContext") {
    return cZoneContext;
  } else {
    // TODO: Chuck something
  }
  return NULL;
}

void TextureSetPerZone::renderPreZone(IZone* zone) {  
  std::map<IZone*, ISpindizzyTextureSet*>::iterator mIterator = cZoneMapping.find(zone);
  cControlledObject->setSpindizzyTextureSet(mIterator != cZoneMapping.end() ? mIterator->second : NULL);
}

void TextureSetPerZone::zoneContextChanged(IMap* map, IZone* zone) {
  cCurrentMap = map;
  cCurrentZone = zone;
}

std::vector<ICommandInfo*> TextureSetPerZone::getCommandInfo() {
  return cPluginCommands;
}

void TextureSetPerZone::setEditingContext(IComponentContainer* componentContainer) {
  cChooseTextureSetCommand->setComponentContainer(componentContainer);
}

void TextureSetPerZone::saveData(DOMNodeWriter* node, IMap* map, IZone* zone) {
  std::map<IZone*, ISpindizzyTextureSet*>::iterator mIterator = cZoneMapping.find(zone);
  if (mIterator != cZoneMapping.end()) {
    ISpindizzyTextureSet* mTextureSet = mIterator->second;
    DOMNodeWriter* mTextureSetNode = node->addBranch("TextureSet");
    IPluginRegistry* mPluginRegistry = map->getPluginRegistry();
    std::string mTextureSetName = mPluginRegistry->getInstanceName("SpindizzyTextureSet", mTextureSet);
    mTextureSetNode->addText(mTextureSetName);
  }
}

void TextureSetPerZone::loadData(DOMNodeWrapper* node, IPluginRegistry* pluginRegistry, IZone* zone) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "TextureSet") {
      std::string mTextureSetName = mNode->getStringValue();
      std::string mPluginType("SpindizzyTextureSet");
      IPlugin* mPlugin = pluginRegistry->getPlugin(mPluginType, mTextureSetName);
      if (mPlugin != NULL) {
        ISpindizzyTextureSet* mTextureSet = dynamic_cast<ISpindizzyTextureSet*>(mPlugin);
        if (mTextureSet != NULL) {
          cZoneMapping[zone] = mTextureSet;
        } else {
          std::cout << "Warning: dynamic_cast for \"" << mTextureSetName << "\" failed!" << std::endl;
        }
      } else {
        std::cout << "Warning: Texture set \"" << mTextureSetName << "\" specified for zone is undefined!" << std::endl;
      }
    }
  }
}

std::vector<IDynamicElement*> TextureSetPerZone::getPreLoopCommands() {
  std::vector<IDynamicElement*> mBackgroundFader;
  mBackgroundFader.push_back(this);
  return mBackgroundFader;
}

void TextureSetPerZone::update(int ticks) {
  if (cProgressBackgroundColour < 1.0f) {
    cProgressBackgroundColour += 0.0025f * ticks;
    if (cProgressBackgroundColour > 1.0f) {
      cProgressBackgroundColour = 1.0f;
    }
    float mRed   = cPreviousBackgroundColour.getRed()   + (cTargetBackgroundColour.getRed()   - cPreviousBackgroundColour.getRed())   * cProgressBackgroundColour;
    float mGreen = cPreviousBackgroundColour.getGreen() + (cTargetBackgroundColour.getGreen() - cPreviousBackgroundColour.getGreen()) * cProgressBackgroundColour;
    float mBlue  = cPreviousBackgroundColour.getBlue()  + (cTargetBackgroundColour.getBlue()  - cPreviousBackgroundColour.getBlue())  * cProgressBackgroundColour;
    glClearColor(mRed, mGreen, mBlue, 0.0f);
  }
}

void TextureSetPerZone::zoneContextChanged(IZone* zone) {
  std::map<IZone*, ISpindizzyTextureSet*>::iterator i = cZoneMapping.find(zone);
  if (i != cZoneMapping.end()) {
    ISpindizzyTexture* mBackgroundTexture = i->second->getTexture(ISpindizzyTextureSet::BACKGROUND);
    switch (mBackgroundTexture->getMapping()) {
      case ISpindizzyTexture::PLAIN_COLOUR: {
        cPreviousBackgroundColour.set(
            cPreviousBackgroundColour.getRed()   + (cTargetBackgroundColour.getRed()   - cPreviousBackgroundColour.getRed())   * cProgressBackgroundColour,
            cPreviousBackgroundColour.getGreen() + (cTargetBackgroundColour.getGreen() - cPreviousBackgroundColour.getGreen()) * cProgressBackgroundColour,
            cPreviousBackgroundColour.getBlue()  + (cTargetBackgroundColour.getBlue()  - cPreviousBackgroundColour.getBlue())  * cProgressBackgroundColour,
            0.0f
        );
        cTargetBackgroundColour = *mBackgroundTexture->getColour(0.0f, 0.0f);
        cProgressBackgroundColour = 0.0f;
        break;
      }

      default: {
        std::cout << "Background type not supported" << std::endl;
        break;
      }
    }
  }
}

TextureSetPerZone::ChooseTextureSetCommand::ChooseTextureSetCommand(TextureSetPerZone* parent) {
  cParent = parent;
  cComponentContainer = NULL;
}

void TextureSetPerZone::ChooseTextureSetCommand::setComponentContainer(IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
}

void TextureSetPerZone::ChooseTextureSetCommand::execute() {
  IHUDComponent* mComponent = new TextureSetChooserComponent(cComponentContainer, cParent, cParent->cTexturePalette);
  cComponentContainer->addComponent(mComponent);
}

extern "C" IPlugin* create() {
  return new TextureSetPerZone();
}

extern "C" void destroy(IPlugin* textureSetPerZone) {
  delete textureSetPerZone;
}

