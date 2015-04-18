/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "SpindizzyLiftConfigurationComponent.h"

SpindizzyLiftConfigurationComponent::SpindizzyLiftConfigurationComponent(IComponentContainer* componentContainer, SpindizzyLiftProperties* properties) : ResizableDialog(componentContainer, "Configure Lift", -0.8f, 0.4f, 1.0f, 1.0f) {
  cProperties = properties;

  // Place the close button in the bottom right
  EdgeRelation* mInsideDialog = new EdgeRelation(this, EdgeRelation::INSIDE);
  Button* mCloseButton = new Button(NULL, new CloseDialogCommand(this), "Close");
  ComponentEdgeLayout* mCloseButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideDialog, mInsideDialog, mCloseButton);
  mCloseButton->setBoundsCalculator(mCloseButtonLayout);

  // Place the apply button next to the close button
  EdgeRelation* mAdjacentCloseButton = new EdgeRelation(mCloseButton, EdgeRelation::OUTSIDE);
  Button* mApplyButton = new Button(NULL, new ApplyCommand(this), "Apply");
  ComponentEdgeLayout* mApplyButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideDialog, mAdjacentCloseButton, mApplyButton);
  mApplyButton->setBoundsCalculator(mApplyButtonLayout);

  // Create a grid for holding the fields to set
  FlexibleGridLayoutComponent* mFormFields = new FlexibleGridLayoutComponent(2, 4);

  unsigned int mUpSpeed     = properties->getUpSpeed();
  unsigned int mDownSpeed   = properties->getDownSpeed();
  unsigned int mTopDelay    = properties->getTopDelay();
  unsigned int mBottomDelay = properties->getBottomDelay();

  cUpSpeedField     = new TextFieldComponent(toString(mUpSpeed));
  cDownSpeedField   = new TextFieldComponent(toString(mDownSpeed));
  cTopDelayField    = new TextFieldComponent(toString(mTopDelay));
  cBottomDelayField = new TextFieldComponent(toString(mBottomDelay));

  mFormFields->addComponent(new TextLabelComponent("Up Speed:"),     0, 3);
  mFormFields->addComponent(new TextLabelComponent("Down Speed:"),   0, 2);
  mFormFields->addComponent(new TextLabelComponent("Top Delay:"),    0, 1);
  mFormFields->addComponent(new TextLabelComponent("Bottom Delay:"), 0, 0);
  mFormFields->addComponent(cUpSpeedField,                           1, 3);
  mFormFields->addComponent(cDownSpeedField,                         1, 2);
  mFormFields->addComponent(cTopDelayField,                          1, 1);
  mFormFields->addComponent(cBottomDelayField,                       1, 0);
  ScrollableContainer* mFormContainer = new ScrollableContainer();
  mFormContainer->setRootComponent(mFormFields);
  ComponentEdgeLayout* mFormLayout = new ComponentEdgeLayout(mInsideDialog, mInsideDialog, mAdjacentCloseButton, mInsideDialog, NULL);
  mFormContainer->setBoundsCalculator(mFormLayout);
  
  addComponent(mCloseButton);
  addComponent(mApplyButton);
  addComponent(mFormContainer);
  setFocusedComponent(mApplyButton);
}

std::string SpindizzyLiftConfigurationComponent::toString(unsigned int i) {
  std::stringstream out;
  out << i;
  return out.str();
}

SpindizzyLiftConfigurationComponent::ApplyCommand::ApplyCommand(SpindizzyLiftConfigurationComponent* parent) {
  cParent = parent;
}

void SpindizzyLiftConfigurationComponent::ApplyCommand::execute() {
  cParent->cProperties->setUpSpeed(atoi(cParent->cUpSpeedField->getText().c_str()));
  cParent->cProperties->setDownSpeed(atoi(cParent->cDownSpeedField->getText().c_str()));
  cParent->cProperties->setTopDelay(atoi(cParent->cTopDelayField->getText().c_str()));
  cParent->cProperties->setBottomDelay(atoi(cParent->cBottomDelayField->getText().c_str()));
}

bool SpindizzyLiftConfigurationComponent::inputResizableDialogContent(SDL_Event&) {
  return false;
}

void SpindizzyLiftConfigurationComponent::renderResizableDialogContent() {
  // Nothing to do.
}

void SpindizzyLiftConfigurationComponent::updateResizableDialogContent(int ticks) {
  // Nothing to do.
}

