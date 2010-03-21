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
#include "EntityClassInstancesComponent.h"

EntityClassInstancesComponent::EntityClassInstancesComponent(IEntityClass* entityClass, IComponentContainer* componentContainer, IInstanceSelectionListener* listener) : ResizableDialog(componentContainer, "Engine/Default/ControlLoop/SimpleEditor/EntityClassInstances") {

  // Setup stuff we need for the dialog
  cEntityClass = entityClass;
  cListener = listener;
  std::vector<std::string*> mImplementationsList = entityClass->getImplementations();
  std::vector<std::string*> mInstances = cEntityClass->getInstances();

  // Set up active components of the dialog
  setComponentAction("removeButton", new RemoveInstanceCommand(this));
  setComponentAction("createButton", new CreateInstanceCommand(this));
  setComponentAction("closeButton", new CloseCommand(this));
  setComponentAction("configureButton", new ConfigureInstanceCommand(this));
  cImplementationsList = getListBox("implementationsList");
  cInstancesList = getListBox("instancesList");
  for (unsigned int i = 0; i < mImplementationsList.size(); i++) {
    cImplementationsList->addItem(*mImplementationsList[i]);
  }
  for (unsigned int i = 0; i < mInstances.size(); i++) {
    cInstancesList->addItem(*mInstances[i]);
  }
}

void EntityClassInstancesComponent::updateResizableDialogContent(int milliseconds) {
  // Nothing to do.
}

void EntityClassInstancesComponent::renderResizableDialogContent() {
  // Nothing to do.
}

bool EntityClassInstancesComponent::inputResizableDialogContent(SDL_Event& event) {
  return false;
}

/*********************\
 * Dialogue commands *
\*********************/
EntityClassInstancesComponent::ConfigureInstanceCommand::ConfigureInstanceCommand(EntityClassInstancesComponent* parent) {
  cParent = parent;
}

void EntityClassInstancesComponent::ConfigureInstanceCommand::execute() {
  // TODO: Should use getStringValu()
  std::string mSelectedInstance = cParent->cInstancesList->getSelectedItem();
  cParent->cEntityClass->configure(mSelectedInstance);
}

EntityClassInstancesComponent::RemoveInstanceCommand::RemoveInstanceCommand(EntityClassInstancesComponent* parent) {
  cParent = parent;
}

void EntityClassInstancesComponent::RemoveInstanceCommand::execute() {
  // TODO: Should use getStringValu()
  std::string mSelectedInstance = cParent->cInstancesList->getSelectedItem();
  cParent->cEntityClass->remove(mSelectedInstance);
}

EntityClassInstancesComponent::CreateInstanceCommand::CreateInstanceCommand(EntityClassInstancesComponent* parent) {
  cParent = parent;
}

void EntityClassInstancesComponent::CreateInstanceCommand::execute() {
  std::cout << "Getting selected..." << std::endl;
  std::string mImplementation = cParent->cImplementationsList->getSelectedItem();
  std::cout << "Getting name..." << std::endl;
  std::string mInstanceName = cParent->getStringValue("instanceField");
  std::cout << "Creating instance \"" << mInstanceName << "\"..." << std::endl;
  cParent->cEntityClass->instantiate(mImplementation, mInstanceName);
  std::cout << "Done..." << std::endl;
}

EntityClassInstancesComponent::CloseCommand::CloseCommand(EntityClassInstancesComponent* parent) {
  cParent = parent;
}

void EntityClassInstancesComponent::CloseCommand::execute() {
  if (cParent->cListener != NULL) {
    std::string mSelectedInstance = cParent->cInstancesList->getSelectedItem();
    cParent->cListener->itemSelected(mSelectedInstance);
  }
  cParent->close();
}
