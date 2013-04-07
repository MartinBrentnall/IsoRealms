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
  cDefaultTextureSetCommand = new DefaultTextureSetCommand(this);
}

void TextureSetPerZone::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  IArgumentSource* mArgumentSource = new ArgumentSourceCustom<TextureSetPerZone>(this);
  runtimeContext->registerArgument("ThemedZones", "ThemedZones", mArgumentSource);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Theme") {
      std::string mThemeName = mNode->getAttribute("name");
      Theme* mTheme = new Theme(this);
      runtimeContext->add(mTheme, mNode);
      runtimeContext->addListener(static_cast<IResourceUseListener<ITexture>*>(mTheme));
      runtimeContext->addListener(static_cast<IResourceUseListener<IColour>*>(mTheme));
      cThemes[mThemeName] = mTheme;
      createActualResources(mNode, runtimeContext);
    } else if (mValueAsString == "Instance") {
      std::string mInstanceName = mNode->getAttribute("name");
      BackgroundChanger* mBackground = new BackgroundChanger(this);
      cNamedInstances[mInstanceName] = mBackground;
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<BackgroundChanger>(mBackground);
      runtimeContext->registerArgument("ColouredBackground", mInstanceName, mArgumentSource);
    }
  }
  runtimeContext->add(this, "Background", NULL);
}

void TextureSetPerZone::createActualResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      std::string mTextureName = mNode->getAttribute("type");
      createThemeTexture(mTextureName, runtimeContext);
    } else if (mValueAsString == "Colour") {
      std::string mColourName = mNode->getAttribute("type");
      createThemeColour(mColourName, runtimeContext);
    }
  }
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

std::string TextureSetPerZone::getThemeName(Theme* theme) {
  for (std::map<std::string, Theme*>::iterator i = cThemes.begin(); i != cThemes.end(); i++) {
    if (i->second == theme) {
      return i->first;
    }
  }
  // TODO: Throw
  std::cout << "WARNING: Theme name could not be found" << std::endl;
  exit(1);
}

IColour* TextureSetPerZone::getColour(IZone* zone, ThemeColour* themeColour) {
  std::map<IZone*, Theme*>::iterator i = cZoneThemes.find(zone);
  if (i != cZoneThemes.end()) {
    return i->second->getColour(themeColour);
  }
  return NULL;
}

void TextureSetPerZone::saveData(DOMNodeWriter* node, IMap* map, IZone* zone) {
  std::map<IZone*, Theme*>::iterator i = cZoneThemes.find(zone);
  if (i != cZoneThemes.end()) {
    DOMNodeWriter* mTextureSetNode = node->addBranch("TextureSet");
    std::string mThemeName = getThemeName(i->second);
    mTextureSetNode->addText(mThemeName);
  }
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

void TextureSetPerZone::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (std::map<std::string, Theme*>::iterator i = cThemes.begin(); i != cThemes.end(); i++) {
    DOMNodeWriter* mThemeNode = node->addBranch("Theme");
    mThemeNode->addAttribute("name", i->first);
    i->second->save(mThemeNode, resourceLocator);
  }
}

void TextureSetPerZone::createThemeTexture(const std::string& type, IRuntimeContext* runtimeContext) {
  if (cTextures.find(type) == cTextures.end()) {
    cTextures[type] = new ThemeTexture();
    runtimeContext->add(cTextures[type], type);
  }
}

void TextureSetPerZone::createThemeColour(const std::string& type, IRuntimeContext* runtimeContext) {
  if (cColours.find(type) == cColours.end()) {
    cColours[type] = new ThemeColour();
    IArgumentSource* mArgumentSource = new ArgumentSourceCustom<ThemeColour>(cColours[type]);
    runtimeContext->add(cColours[type], type);
    runtimeContext->registerArgument("ThemeColour", type, mArgumentSource);
  }
}

ThemeTexture* TextureSetPerZone::getThemeTexture(const std::string& type) {
  return cTextures[type];
}

ThemeColour* TextureSetPerZone::getThemeColour(const std::string& type) {
  return cColours[type];
}

std::string TextureSetPerZone::getThemeElement(ThemeTexture* themeTexture) {
  for (std::map<std::string, ThemeTexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (i->second == themeTexture) {
      return i->first;
    }
  }
  // TODO: Throw
  std::cout << "WARNING: Theme element not found" << std::endl;
  exit(1);
}

std::string TextureSetPerZone::getThemeElement(ThemeColour* themeColour) {
  for (std::map<std::string, ThemeColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
    if (i->second == themeColour) {
      return i->first;
    }
  }
  // TODO: Throw
  std::cout << "WARNING: Theme element not found" << std::endl;
  exit(1);
}

TextureSetPerZone::DefaultTextureSetCommand::DefaultTextureSetCommand(TextureSetPerZone* parent) {
  cParent = parent;
}

void TextureSetPerZone::DefaultTextureSetCommand::update(unsigned int milliseconds) {
  cParent->renderPreZone(cParent->cCurrentZone);
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

IPlugin* TextureSetPerZone::getElementSet() {
  return this;
}

IElement* TextureSetPerZone::getElement(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container) {
  std::string mInstance = node->getAttribute("instance");
  if (mInstance != "") {
    return cNamedInstances[mInstance];
  } else {
    return new BackgroundChanger(this);
  }
}

void TextureSetPerZone::setEditingContext(IEditingContext* editingContext, IResourceManager* resourceManager) {
  resourceManager->addResourceType(this, "Themed Texture");
  cComponentContainer = editingContext->getComponentContainer();
}

void TextureSetPerZone::configureElement() {
  // TODO: Implement this
}

bool TextureSetPerZone::input(SDL_Event&) {
  // Nothing to do
  return false;
}

void TextureSetPerZone::renderEditingPreview() {
  // Nothing to do
}

void TextureSetPerZone::renderIcon() {
  // TODO: Implement this
}

void TextureSetPerZone::updateIcon(unsigned int) {
  // TODO: Implement this
}

void TextureSetPerZone::destroy(IElement* element) {
  delete element;
}

IElementHandler* TextureSetPerZone::getElementHandler() {
  return NULL;
}

void TextureSetPerZone::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  // Nothing to do
}

void TextureSetPerZone::createResource(IResourceAccessor* resources) {
  // TODO: Implement this
}

void TextureSetPerZone::editResource(ITexture* texture, IResourceAccessor* resources) {
  // TODO: Implement this
}

void TextureSetPerZone::removeResource(ITexture* texture, IResourceAccessor* resources, IResourceInstanceListener<ITexture>* listener) {
  for (std::map<std::string, ThemeTexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (i->second == texture) {
      ThemeTexture* mTexture = i->second;
      for (std::map<std::string, Theme*>::iterator j = cThemes.begin(); j != cThemes.end(); j++) {
	j->second->removeTexture(mTexture);
      }
      listener->resourcePendingDestruction(texture);
      delete mTexture;
      cTextures.erase(i);
      return;
    }
  }
}

extern "C" IPlugin* create() {
  return new TextureSetPerZone();
}

extern "C" void destroy(IPlugin* textureSetPerZone) {
  delete textureSetPerZone;
}

