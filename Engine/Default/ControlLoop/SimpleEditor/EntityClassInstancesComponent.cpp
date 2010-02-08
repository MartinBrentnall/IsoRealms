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

//TODO: CLASS REQUIRES REFACTOR FOR NEW COMPONENT FRAMEWORK!
EntityClassInstancesComponent::EntityClassInstancesComponent(IInstantiable* instantiator, IComponentContainer* componentContainer, float x, float y, IInstanceSelectionListener* listener) : ResizableDialog(componentContainer, getTitle(instantiator), x, y, 1.26f, 0.78f) {
//  std::vector<std::string>* mImplementationsList = System::getFileList("/usr/share/IsoRealms/Elements/", "*");
  std::vector<std::string*> mImplementationsList = instantiator->getImplementations();

  ICommand* mConfigureInstanceCommand = new ConfigureInstanceCommand(this);
  ICommand* mRemoveInstanceCommand = new RemoveInstanceCommand();
  ICommand* mCreateInstanceCommand = new CreateInstanceCommand(this);
  ICommand* mCloseCommand = new CloseCommand(this);

  cInstantiator = instantiator;
  cListener = listener;
 
  // Put "close" button in bottom right.
  EdgeRelation* mInsideDialogue = new EdgeRelation(this, EdgeRelation::INSIDE);
  Button* mCloseButton = new Button(NULL, mCloseCommand, cListener == NULL ? "Close" : "Select Instance");
  IComponentBoundsCalculator* mCloseButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideDialogue, mInsideDialogue, mCloseButton);
  mCloseButton->setBoundsCalculator(mCloseButtonLayout);

  // Put a grid of two cells above the "close" button.
  EdgeRelation* mAdjacentCloseButton = new EdgeRelation(mCloseButton, EdgeRelation::OUTSIDE);
  IComponentBoundsCalculator* mGridLayout = new ComponentEdgeLayout(mInsideDialogue, mInsideDialogue, mAdjacentCloseButton, mInsideDialogue, NULL);
  cGridLayout = new GridLayoutComponent(2, 1, mGridLayout);
  IComponentBoundsCalculator* mLeftCell = cGridLayout->getCellLayout(0, 0);
  IComponentBoundsCalculator* mRightCell = cGridLayout->getCellLayout(1, 0);

  EdgeRelation* mInsideLeftCell = new EdgeRelation(mLeftCell, EdgeRelation::ALIGNED);
  EdgeRelation* mInsideRightCell = new EdgeRelation(mRightCell, EdgeRelation::ALIGNED);

  // Put "new instance" button at bottom right of right cell.
  Button* mNewInstanceButton = new Button(NULL, mCreateInstanceCommand, "New Instance");
  IComponentBoundsCalculator* mNewInstanceButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideRightCell, mInsideRightCell, mNewInstanceButton);
  mNewInstanceButton->setBoundsCalculator(mNewInstanceButtonLayout);

  // Put instance name label above buttons in right cell.
  EdgeRelation* mAdjacentNewButton = new EdgeRelation(mNewInstanceButton, EdgeRelation::OUTSIDE);
  TextLabelComponent* mInstanceNameLabel = new TextLabelComponent("Instance Name:");
  IComponentBoundsCalculator* mInstanceNameLayout = new ComponentEdgeLayout(NULL, mInsideRightCell, mAdjacentNewButton, NULL, mInstanceNameLabel);
  mInstanceNameLabel->setBoundsCalculator(mInstanceNameLayout);

  // Put instance name text field above buttons in right cell and to right of instance name label.
  EdgeRelation* mAdjacentInstanceNameLabel = new EdgeRelation(mInstanceNameLabel, EdgeRelation::OUTSIDE, 0.0f);
  cInstanceNameInputField = new TextFieldComponent();
  IComponentBoundsCalculator* mInstanceNameFieldLayout = new ComponentEdgeLayout(NULL, mAdjacentInstanceNameLabel, mAdjacentNewButton, mInsideRightCell, cInstanceNameInputField);
  cInstanceNameInputField->setBoundsCalculator(mInstanceNameFieldLayout);

  // Put implementations label at top of right cell
  TextLabelComponent* mImplementationsLabel = new TextLabelComponent("Implementations:");
  IComponentBoundsCalculator* mImplementationsLabelLayout = new ComponentEdgeLayout(mInsideRightCell, mInsideRightCell, NULL, NULL, mImplementationsLabel);
  mImplementationsLabel->setBoundsCalculator(mImplementationsLabelLayout);

  // Put implementations list box above instance name and below implementations label in right cell.
  EdgeRelation* mAdjacentInstanceNameField = new EdgeRelation(cInstanceNameInputField, EdgeRelation::OUTSIDE);
  EdgeRelation* mAdjacentImplementationsLabel = new EdgeRelation(mImplementationsLabel, EdgeRelation::OUTSIDE, 0.0f);
  IComponentBoundsCalculator* mImplementationsListLayout = new ComponentEdgeLayout(mAdjacentImplementationsLabel, mInsideRightCell, mAdjacentInstanceNameField, mInsideRightCell, NULL);
  cImplementationsList = new ImplementationsListComponent(mImplementationsList);
  cImplementationsList->setBoundsCalculator(mImplementationsListLayout);

  // Put "configure..." button at bottom right of left cell.
  Button* mConfigureButton = new Button(NULL, mConfigureInstanceCommand, "Configure...");
  IComponentBoundsCalculator* mConfigureButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideLeftCell, mInsideLeftCell, mConfigureButton);
  mConfigureButton->setBoundsCalculator(mConfigureButtonLayout);

  // Put "remove instance" button to the left of the "configure..." button.
  EdgeRelation* mAdjacentConfigureButton = new EdgeRelation(mConfigureButton, EdgeRelation::OUTSIDE);
  EdgeRelation* mAlignToConfigureButton = new EdgeRelation(mConfigureButton, EdgeRelation::ALIGNED);
  Button* mRemoveInstanceButton = new Button(NULL, mRemoveInstanceCommand, "Remove Instance");
  IComponentBoundsCalculator* mRemoveInstanceButtonLayout = new ComponentEdgeLayout(mAlignToConfigureButton, NULL, mAlignToConfigureButton, mAdjacentConfigureButton, mRemoveInstanceButton);
  mRemoveInstanceButton->setBoundsCalculator(mRemoveInstanceButtonLayout);

  // Put instances label at top of left cell
  TextLabelComponent* mInstancesLabel = new TextLabelComponent("Instances:");
  IComponentBoundsCalculator* mInstancesLabelLayout = new ComponentEdgeLayout(mInsideLeftCell, mInsideLeftCell, NULL, NULL, mInstancesLabel);
  mInstancesLabel->setBoundsCalculator(mInstancesLabelLayout);

  // Put instances list box above "configure..." button and below instances label in left cell.
  EdgeRelation* mAdjacentInstancesLabel = new EdgeRelation(mInstancesLabel, EdgeRelation::OUTSIDE, 0.0f);
  cInstancesList = new InstancesListComponent(cInstantiator);
  IComponentBoundsCalculator* mInstancesListLayout = new ComponentEdgeLayout(mAdjacentInstancesLabel, mInsideLeftCell, mAdjacentConfigureButton, mInsideLeftCell, NULL);
  cInstancesList->setBoundsCalculator(mInstancesListLayout);

  addComponent(mCloseButton);
  addComponent(mNewInstanceButton);
  addComponent(mInstanceNameLabel);
  addComponent(cImplementationsList);
  addComponent(mImplementationsLabel);

  addComponent(mRemoveInstanceButton);
  addComponent(mConfigureButton);
  addComponent(cInstancesList);
  addComponent(mInstancesLabel);
  addComponent(cInstanceNameInputField);

  setFocusedComponent(cImplementationsList);
}

std::string* EntityClassInstancesComponent::getTitle(IInstantiable* instantiator) {
  return new std::string(instantiator->getEntityClassName() + " Instances");
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
  std::string* mSelectedInstance = cParent->cInstancesList->getSelectedInstance();
  cParent->cInstantiator->configure(*mSelectedInstance);
}

void EntityClassInstancesComponent::RemoveInstanceCommand::execute() {
}

EntityClassInstancesComponent::CreateInstanceCommand::CreateInstanceCommand(EntityClassInstancesComponent* parent) {
  cParent = parent;
}

void EntityClassInstancesComponent::CreateInstanceCommand::execute() {
  std::string mImplementation = cParent->cImplementationsList->getSelectedImplementation();
  std::string mInstanceName = cParent->cInstanceNameInputField->getText();
  cParent->cInstantiator->instantiate(mImplementation, mInstanceName);
}

EntityClassInstancesComponent::CloseCommand::CloseCommand(EntityClassInstancesComponent* parent) {
  cParent = parent;
}

void EntityClassInstancesComponent::CloseCommand::execute() {
  if (cParent->cListener != NULL) {
    std::string* mSelectedInstance = cParent->cInstancesList->getSelectedInstance();
    cParent->cListener->itemSelected(*mSelectedInstance);
  }
  cParent->close();
}
