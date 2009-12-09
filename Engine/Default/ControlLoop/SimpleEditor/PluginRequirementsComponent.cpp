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
// TODO: Rename "SupportedPluginsComponent.h"
#include "PluginRequirementsComponent.h"

IFont* PluginRequirementsComponent::cFont = NULL;

void PluginRequirementsComponent::setFont(IFont* font) {
  cFont = font;
}

PluginRequirementsComponent::PluginRequirementsComponent(IComponentContainer* componentContainer, PluginRegistry* pluginRegistry, IPluginSupport* pluginSupport, float x, float y) : RectangleComponent(componentContainer, x, y, 1.0f, 1.0f) {
  cPluginRegistry = pluginRegistry;
  cSelectedPlugin = 0;
  cChooseInstanceComponent = NULL;
  cComponentContainer = componentContainer;
  cPluginSupport = pluginSupport;

  // Populate supported plugin.
  // TODO: Change to "plug sockets"
  cSupportedPlugins = cPluginSupport->getPlugSockets();
}

void PluginRequirementsComponent::updateContent(int milliseconds) {
  // TODO: Anything to do in myself?
  if (cChooseInstanceComponent != NULL) {
    cChooseInstanceComponent->update(milliseconds);
  }
}

void PluginRequirementsComponent::renderContent() {
  std::string mTitle = "\"" + cPluginSupport->getName() + "\" supports the following plugin:";
  float mLine = getTop() - 0.05f;
  cFont->print(getLeft() + 0.02f, mLine, 0.02f, 0, mTitle.c_str());
  mLine -= 0.08f;
  for (unsigned int i = 0; i < cSupportedPlugins.size(); i++) {
//    float mLoadedMultiplier = ElementSetRegistry::isElementSetLoaded(cSupportedPlugins[i]) ? 1.0f : 0.3f;
    float mLoadedMultiplier = 1.0f; // TODO: Do we need this?
    if (i == cSelectedPlugin) {
      glColor3f(0.0f * mLoadedMultiplier , 1.0f * mLoadedMultiplier, 0.0f * mLoadedMultiplier);
    } else {
      glColor3f(1.0f * mLoadedMultiplier, 1.0f * mLoadedMultiplier, 1.0f * mLoadedMultiplier);
    }
    cFont->print(getLeft() + 0.02f, mLine, 0.02f, 0, cSupportedPlugins[i]->getType().c_str());
 
// TODO: Plug socket    string mPluginFulfillment = cPluginSupport->getPlugin(cSupportedPlugins[i]);
//    cFont->print(getLeft() + 0.52f, mLine, 0.02f, 0, mPluginFulfillment.c_str());
    mLine -= 0.05f;
  }

  if (cChooseInstanceComponent != NULL) {
    cChooseInstanceComponent->render();
  }
}

bool PluginRequirementsComponent::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_DOWN: {
      if (cSelectedPlugin < cSupportedPlugins.size() - 1) {
        cSelectedPlugin++;
      }
      return true;
    }
  
    case SDLK_UP: {
      if (cSelectedPlugin > 0) {
        cSelectedPlugin--;
      }
      return true;
    }
  
    case SDLK_RETURN: {
      IHUDComponent* mChooseInstanceComponent = new ChoosePluginInstanceComponent(cComponentContainer, cPluginRegistry, cPluginSupport, cSupportedPlugins[cSelectedPlugin], getLeft() + 0.05f, getBottom() - 0.05f);
      cComponentContainer->addComponent(mChooseInstanceComponent);
      return true;
    }

    case SDLK_ESCAPE: {
      close();
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool PluginRequirementsComponent::inputContent(SDL_Event& event) {
  if (cChooseInstanceComponent != NULL) {
    if (cChooseInstanceComponent->input(event)) {
      return true;
    }
  } else {
    switch (event.type) {
      case SDL_KEYDOWN: {
        return keyDown(event.key.keysym.sym);
      }
    }
  }
  return false;
}

