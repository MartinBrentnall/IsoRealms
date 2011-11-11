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

TextureSetPerZone::TextureSetPerZone(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cProgressBackgroundColour = 1.0f;
  cDefaultTextureSetCommand = new DefaultTextureSetCommand(this);
  cPreviousBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
  cTargetBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
}

void TextureSetPerZone::setPlugin(PlugSocket* plugSocket, IPlugin* plugin) {
  if (plugSocket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *plugSocket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  }
}

IPlugin* TextureSetPerZone::getPlugin(PlugSocket* plugSocket) {
  if (plugSocket->getType() == "ZoneContext") {
    return cZoneContext;
  } else {
    // TODO: Chuck something
  }
  return NULL;
}

void TextureSetPerZone::renderPreZone(IZone* zone) {
  // TODO: Change the textures
  std::map<IZone*, Theme*>::iterator i = cZoneThemes.find(zone);
  i->second->set();
}

void TextureSetPerZone::zoneContextChanged(IMap* map, IZone* zone) {
  cCurrentMap = map;
  cCurrentZone = zone;
}

void TextureSetPerZone::setEditingContext(IEditingContext* editingContext) {
  std::vector<std::string> mPath;
  mPath.push_back("Choose Texture");
  // TODO: How to identify which element set(s) are being controlled by this plugin?
  mPath.push_back("Zone");
  cComponentContainer = editingContext->getComponentContainer();
  OpenDialogCommand* mChooseTextureSetCommand = new OpenDialogCommand(cComponentContainer, this);
  editingContext->registerCommand(new DefaultCommandInfo(mPath, mChooseTextureSetCommand));
  cBlockLocation = editingContext->getBlockLocation();
}

void TextureSetPerZone::saveData(DOMNodeWriter* node, IMap* map, IZone* zone) {
  // TODO: Implement this
/*  std::map<IZone*, ITextureSet*>::iterator mIterator = cZoneMapping.find(zone);
  if (mIterator != cZoneMapping.end()) {
    ITextureSet* mTextureSet = mIterator->second;
    DOMNodeWriter* mTextureSetNode = node->addBranch("TextureSet");
    IPluginRegistry* mPluginRegistry = map->getPluginRegistry();
    std::string mTextureSetName = mPluginRegistry->getInstanceName("TextureSet", mTextureSet);
    mTextureSetNode->addText(mTextureSetName);
  }*/
}

void TextureSetPerZone::loadData(DOMNodeWrapper* node, IPluginRegistry* pluginRegistry, IZone* zone) {
  // TODO: Implement this
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "TextureSet") {
      std::string mThemeName = mNode->getStringValue();
      Theme* mTheme = cThemes[mThemeName]; // TODO: Error if the theme doesn't exist
      cZoneThemes[zone] = mTheme;
    }
  }
}

void TextureSetPerZone::save(DOMNodeWriter* node) {
}

ThemeTexture* TextureSetPerZone::getThemeTexture(const std::string& type) {
  if (cTextures.find(type) == cTextures.end()) {
    cTextures[type] = new ThemeTexture();
    cRuntimeContext->add(cTextures[type], type);
  }
  return cTextures[type];
}

void TextureSetPerZone::loadTheme(DOMNodeWrapper* node, Theme* theme) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      std::string mType = mNode->getAttribute("type");
      ITexture* mTexture = cRuntimeContext->getTexture(mNode);
      ThemeTexture* mThemeTexture = getThemeTexture(mType);
      theme->registerElement(mThemeTexture, mTexture);
    }
  }      
}

void TextureSetPerZone::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Theme") {
      std::string mThemeName = mNode->getAttribute("name");
      Theme* mTheme = new Theme();
      loadTheme(mNode, mTheme);
      cThemes[mThemeName] = mTheme;
    }
  }
}

std::vector<IDynamicElement*> TextureSetPerZone::getPreLoopCommands() {
  std::vector<IDynamicElement*> mBackgroundFader;
  mBackgroundFader.push_back(this);
  return mBackgroundFader;
}

std::vector<IDynamicElement*> TextureSetPerZone::getPostLoopCommands() {
  std::vector<IDynamicElement*> mZoneTextureSetter;
  mZoneTextureSetter.push_back(cDefaultTextureSetCommand);
  return mZoneTextureSetter;
}

TextureSetPerZone::DefaultTextureSetCommand::DefaultTextureSetCommand(TextureSetPerZone* parent) {
  cParent = parent;
}

void TextureSetPerZone::DefaultTextureSetCommand::update(int milliseconds) {
  cParent->renderPreZone(cParent->cCurrentZone);
}

void TextureSetPerZone::update(int ticks) {
  if (cProgressBackgroundColour < 1.0f) {
    cProgressBackgroundColour += 0.0025f * ticks;
    if (cProgressBackgroundColour > 1.0f) {
      cProgressBackgroundColour = 1.0f;
    }
    float mRed   = cPreviousBackgroundColour->getRed()   + (cTargetBackgroundColour->getRed()   - cPreviousBackgroundColour->getRed())   * cProgressBackgroundColour;
    float mGreen = cPreviousBackgroundColour->getGreen() + (cTargetBackgroundColour->getGreen() - cPreviousBackgroundColour->getGreen()) * cProgressBackgroundColour;
    float mBlue  = cPreviousBackgroundColour->getBlue()  + (cTargetBackgroundColour->getBlue()  - cPreviousBackgroundColour->getBlue())  * cProgressBackgroundColour;
    float mFogColour[4] = {mRed, mGreen, mBlue, 0.0f};
    glFogfv(GL_FOG_COLOR, mFogColour);
    glClearColor(mRed, mGreen, mBlue, 0.0f);
  }
}

void TextureSetPerZone::zoneContextChanged(IZone* zone) {
/*  std::map<IZone*, ITextureSet*>::iterator i = cZoneMapping.find(zone);
  if (i != cZoneMapping.end()) {
    cCurrentZone = zone;
    ITexture* mBackgroundTexture = i->second->getTexture("Background");
    Colour* mNewPreviousBackgroundColour = new Colour(
        cPreviousBackgroundColour->getRed()   + (cTargetBackgroundColour->getRed()   - cPreviousBackgroundColour->getRed())   * cProgressBackgroundColour,
        cPreviousBackgroundColour->getGreen() + (cTargetBackgroundColour->getGreen() - cPreviousBackgroundColour->getGreen()) * cProgressBackgroundColour,
        cPreviousBackgroundColour->getBlue()  + (cTargetBackgroundColour->getBlue()  - cPreviousBackgroundColour->getBlue())  * cProgressBackgroundColour,
        0.0f
    );
    delete cPreviousBackgroundColour;
    delete cTargetBackgroundColour;
    cPreviousBackgroundColour = mNewPreviousBackgroundColour;
    cTargetBackgroundColour = new Colour(*mBackgroundTexture->getColour(0.0f, 0.0f));
    cProgressBackgroundColour = 0.0f;
  }*/
}

IHUDComponent* TextureSetPerZone::createComponent() {
  return new TextureSetChooserComponent(cComponentContainer);
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new TextureSetPerZone(runtimeContext);
}

extern "C" void destroy(IPlugin* textureSetPerZone) {
  delete textureSetPerZone;
}

