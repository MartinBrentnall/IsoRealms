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
#include "ElementSetInstancesComponent.h"

//TODO: CLASS REQUIRES REFACTOR FOR NEW COMPONENT FRAMEWORK!
ElementSetInstancesComponent::ElementSetInstancesComponent(IComponentContainer* componentContainer, PluginRegistry* pluginRegistry, ElementSetRegistry* elementSetRegistry, float x, float y) : ResizableDialog(componentContainer, x, y, 1.26f, 0.78f) {
  std::vector<std::string>* mImplementationsList = System::getFileList("/usr/share/IsoRealms/Elements/", "*");;
  float mOffset = 0.02f;
  ICommand* mConfigureInstanceCommand = new ConfigureInstanceCommand(this);
  ICommand* mRemoveInstanceCommand = new RemoveInstanceCommand();
  ICommand* mCreateInstanceCommand = new CreateInstanceCommand(this);
  ICommand* mCloseCommand = new CloseCommand(this);

  cPluginRegistry = pluginRegistry;
  cElementSetRegistry = elementSetRegistry;

  cGridLayout = new GridLayoutComponent(2, 1, new RectangleBoundsCalculator(this));
  cInstancesList = new InstancesListComponent(cElementSetRegistry);
  cImplementationsList = new ImplementationsListComponent(cInstancesList, IRectangularComponent::RIGHT, mOffset, *mImplementationsList);
  cInstanceNameInputField = new TextFieldComponent(cImplementationsList, IRectangularComponent::BOTTOM, mOffset);
  cConfigureButton = new Button(cInstancesList, IRectangularComponent::BOTTOM, mOffset, mConfigureInstanceCommand, "Configure...");
  cRemoveInstanceButton = new Button(cConfigureButton, IRectangularComponent::RIGHT, mOffset, mRemoveInstanceCommand, "Remove Instance");
  cNewInstanceButton = new Button(cInstanceNameInputField, IRectangularComponent::BOTTOM, mOffset, mCreateInstanceCommand, "New Instance");
  cCloseButton = new Button(cNewInstanceButton, IRectangularComponent::RIGHT, mOffset, mCloseCommand, "Close");

  cGridLayout->setComponentInCell(0, 0, cInstancesList);
  cGridLayout->setComponentInCell(1, 0, cImplementationsList);

  cComponents.push_back(cInstancesList);
  cComponents.push_back(cImplementationsList);
  cComponents.push_back(cInstanceNameInputField);
  cComponents.push_back(cConfigureButton);
  cComponents.push_back(cRemoveInstanceButton);
  cComponents.push_back(cNewInstanceButton);
  cComponents.push_back(cCloseButton);

  cFocusedComponent = cImplementationsList;
}

void ElementSetInstancesComponent::updateResizableDialogContent(int milliseconds) {
  // Nothing to do.
}

void ElementSetInstancesComponent::renderResizableDialogContent() {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->render();
  }
}

void ElementSetInstancesComponent::testFocusChange(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    if (cComponents[i]->contains(mX, mY)) {
      cFocusedComponent = cComponents[i];
    }
  }
}

bool ElementSetInstancesComponent::inputResizableDialogContent(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      testFocusChange(event);
    }
  }
  return cFocusedComponent->input(event);
}

void ElementSetInstancesComponent::configureElementSet() {
  IElementSet* mSelectedElementSet = cInstancesList->getSelectedElementSet();
  if (mSelectedElementSet != NULL) {
    IHUDComponent* mComponent = new PluginRequirementsComponent(cComponentContainer, cPluginRegistry, mSelectedElementSet, getLeft() + 0.05f, getBottom() - 0.05f);
    cComponentContainer->addComponent(mComponent);
  }
}

ElementSetInstancesComponent::ConfigureInstanceCommand::ConfigureInstanceCommand(ElementSetInstancesComponent* parent) {
  cParent = parent;
}

void ElementSetInstancesComponent::ConfigureInstanceCommand::execute() {
  cParent->configureElementSet();
}

void ElementSetInstancesComponent::RemoveInstanceCommand::execute() {
}

ElementSetInstancesComponent::CreateInstanceCommand::CreateInstanceCommand(ElementSetInstancesComponent* parent) {
  cParent = parent;
}

void ElementSetInstancesComponent::CreateInstanceCommand::execute() {
  std::string mImplementation = cParent->cImplementationsList->getSelectedImplementation();
  std::string mInstanceName = cParent->cInstanceNameInputField->getText();
  cParent->cElementSetRegistry->createInstance(mImplementation, mInstanceName);
}

ElementSetInstancesComponent::CloseCommand::CloseCommand(ElementSetInstancesComponent* parent) {
  cParent = parent;
}

void ElementSetInstancesComponent::CloseCommand::execute() {
  cParent->close();
}


