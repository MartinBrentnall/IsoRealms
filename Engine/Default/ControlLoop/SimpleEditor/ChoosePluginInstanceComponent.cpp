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
#include "ChoosePluginInstanceComponent.h"

IFont* ChoosePluginInstanceComponent::cFont = NULL;

void ChoosePluginInstanceComponent::setFont(IFont* font) {
  cFont = font;
}

ChoosePluginInstanceComponent::ChoosePluginInstanceComponent(IComponentContainer* componentContainer, PluginRegistry* pluginRegistry, IPluginSupport* pluginSupport, PlugSocket* socket, float x, float y) : RectangleComponent(componentContainer, x, y, 1.0f, 1.0f) {
  cPluginRegistry = pluginRegistry;
  cPluginSupport = pluginSupport;
  cPlugSocket = socket;
  std::string mType = cPlugSocket->getType();
  cPluginInstances = cPluginRegistry->getInstances(mType);
  cComponentContainer = componentContainer;
  cSelectedInstance = 0;

  ICommand* mNewInstanceCommand = new ChoosePluginImplementationCommand(cComponentContainer, cPluginRegistry, mType, &cX, &cY);
  cOKCommand = new OKCommand(this);
  cCancelCommand = new CancelCommand(this);
  cConfigureCommand = new ConfigurePluginCommand(this);

  float mOffset = 0.02f;
  cNewInstanceButton = new Button(this, IRectangularComponent::BOTTOM, -0.09f, mNewInstanceCommand, "New Instance...");
  cConfigureButton = new Button(cNewInstanceButton, IRectangularComponent::RIGHT, mOffset, cConfigureCommand, "Configure...");
  cOKButton = new Button(cConfigureButton, IRectangularComponent::RIGHT, mOffset, cOKCommand, "OK");
  cCancelButton = new Button(cOKButton, IRectangularComponent::RIGHT, mOffset, cCancelCommand, "Cancel");

  cFocusedComponent = cNewInstanceButton;

  cPluginRegistry->addListener(this);
}

void ChoosePluginInstanceComponent::updateContent(int milliseconds) {
}

void ChoosePluginInstanceComponent::renderContent() {
  std::string mTitle = "Choose a an instance of \"" + cPlugSocket->getType() + "\":";
  float mLine = (getY() + getHeight()) - 0.05f;
  cFont->print(getX() + 0.02f, mLine, 0.02f, 0, mTitle.c_str());
  mLine -= 0.08f;
  for (unsigned int i = 0; i < cPluginInstances.size(); i++) {
    float mMultiplier = i == cSelectedInstance ? 1.0f : 0.4f;
//    IPluginAccessor* mPluginAccessor = cPluginRegistry->getPlugin(cPlugSocket, cPluginInstances[i]);
    glColor3f(1.0f * mMultiplier, 1.0f * mMultiplier, 1.0f * mMultiplier);
    cFont->print(getX() + 0.02f, mLine, 0.02f, 0, cPluginInstances[i].c_str());
    mLine -= 0.05f;
  }
  if (cPluginInstances.empty()) {
    cFont->print(getX() + 0.02f, mLine, 0.02f, 0, "No instances found!");
  }
  cNewInstanceButton->render();
  cConfigureButton->render();
  cOKButton->render();
  cCancelButton->render();
}

bool ChoosePluginInstanceComponent::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_DOWN: {
      if (cSelectedInstance < cPluginInstances.size() - 1) {
        cSelectedInstance++;
      }
      return true;
    }
  
    case SDLK_UP: {
      if (cSelectedInstance > 0) {
        cSelectedInstance--;
      }
      return true;
    }
  
    case SDLK_RETURN: {
      if (!cPluginInstances.empty()) {
        cOKCommand->execute();
      }
      return true;
    }

    case SDLK_ESCAPE: {
      cCancelCommand->execute();
      return true;
    }

    default: {
      return false;
    }
  }
}

bool ChoosePluginInstanceComponent::inputContent(SDL_Event& event) {
  if (cNewInstanceButton->input(event) || cOKButton->input(event) || cCancelButton->input(event) || cConfigureButton->input(event)) {
    return true;
  }
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void ChoosePluginInstanceComponent::makeSelection() {
  std::string mInstanceName = cPluginInstances[cSelectedInstance];
  std::string mType = cPlugSocket->getType();
  IPlugin* mPlugin = cPluginRegistry->getPlugin(mType, mInstanceName);
  cPluginSupport->setPlugin(cPlugSocket, mPlugin);
}

void ChoosePluginInstanceComponent::configureSelectedPlugin() {
  std::string mInstanceName = cPluginInstances[cSelectedInstance];
  std::string mType = cPlugSocket->getType();
  IPlugin* mPlugin = cPluginRegistry->getPlugin(mType, mInstanceName);
  PluginRequirementsComponent* mSupportedPluginsComponent = new PluginRequirementsComponent(cComponentContainer, cPluginRegistry, mPlugin, getX() + 0.05f, getY() - 0.05f);
  cComponentContainer->addComponent(mSupportedPluginsComponent);
}

void ChoosePluginInstanceComponent::pluginInstanceAdded(PluginRegistry* pluginRegistry, std::string, std::string) {
  std::string mType = cPlugSocket->getType();
  cPluginInstances = pluginRegistry->getInstances(mType);
}

void ChoosePluginInstanceComponent::pluginInstanceRemoved(IPlugin* instance, std::string type) {
// TODO: This is wrong; we must handle removal in another way (e.g. just removing the instance from cPluginInstances.
  //  cPluginInstances = pluginRegistry->getInstances(type);
}

ChoosePluginInstanceComponent::OKCommand::OKCommand(ChoosePluginInstanceComponent* parent) {
  cParent = parent;
}

void ChoosePluginInstanceComponent::OKCommand::execute() {
  cParent->makeSelection();
  cParent->close();
}

ChoosePluginInstanceComponent::CancelCommand::CancelCommand(ChoosePluginInstanceComponent* parent) {
  cParent = parent;
}

void ChoosePluginInstanceComponent::CancelCommand::execute() {
  cParent->close();
}

ChoosePluginInstanceComponent::ConfigurePluginCommand::ConfigurePluginCommand(ChoosePluginInstanceComponent* parent) {
  cParent = parent;
}

void ChoosePluginInstanceComponent::ConfigurePluginCommand::execute() {
  cParent->configureSelectedPlugin();
}
