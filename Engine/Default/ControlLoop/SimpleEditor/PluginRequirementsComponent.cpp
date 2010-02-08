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

PluginRequirementsComponent::PluginRequirementsComponent(IComponentContainer* componentContainer, PluginRegistry* pluginRegistry, IPluginSupport* pluginSupport, float x, float y) : ResizableDialog(componentContainer, getTitle(pluginSupport), x, y, 1.0f, 1.0f) {
  cPluginRegistry = pluginRegistry;
  cComponentContainer = componentContainer;
  cPluginSupport = pluginSupport;

  ICommand* mCloseCommand = new CloseCommand(this);

  // Populate supported plugin.
  // TODO: Change to "plug sockets"
  cSupportedPlugins = cPluginSupport->getPlugSockets();

  // Put "close" button in bottom right.
  EdgeRelation* mInsideDialog = new EdgeRelation(this, EdgeRelation::INSIDE);
  Button* mCloseButton = new Button(NULL, mCloseCommand, "Close");
  IComponentBoundsCalculator* mCloseButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideDialog, mInsideDialog, mCloseButton);
  mCloseButton->setBoundsCalculator(mCloseButtonLayout);

  // Put label in top left
  TextLabelComponent* mTitleLabel = new TextLabelComponent("\"" + cPluginSupport->getName() + "\" supports the following plugins:");
  IComponentBoundsCalculator* mTitleLayout = new ComponentEdgeLayout(mInsideDialog, mInsideDialog, NULL, NULL, mTitleLabel);
  mTitleLabel->setBoundsCalculator(mTitleLayout);

  // Put table below label (TODO: and above close button)
  FlexibleGridLayoutComponent* mRequirementsTable = new FlexibleGridLayoutComponent(4, cSupportedPlugins.size() + 1);

  TextLabelComponent* mTypeHeading = new TextLabelComponent("Type");
  TextLabelComponent* mSelectedHeading = new TextLabelComponent("Selected");
  mRequirementsTable->addComponent(mTypeHeading,     TYPE_COLUMN,     cSupportedPlugins.size());
  mRequirementsTable->addComponent(mSelectedHeading, INSTANCE_COLUMN, cSupportedPlugins.size());

  for (unsigned int i = 0; i < cSupportedPlugins.size(); i++) {
    std::string mPluginType = cSupportedPlugins[i]->getType();
    TextLabelComponent* mTypeLabel = new TextLabelComponent(mPluginType);
    IPlugin* mPlugin = cPluginSupport->getPlugin(cSupportedPlugins[i]);
    std::string mInstanceName = pluginRegistry->getInstanceName(mPluginType, mPlugin);
    TextLabelComponent* mSelectedInstanceLabel = new TextLabelComponent(mInstanceName);
    ICommand* mChoosePluginInstanceCommand = new ChoosePluginImplementationCommand(cPluginSupport, cSupportedPlugins[i], componentContainer, pluginRegistry, mPluginType);
    Button* mChoosePluginButton = new Button(NULL, mChoosePluginInstanceCommand, "Choose...");
    ICommand* mResetSocketCommand = new ResetSocketCommand(cPluginSupport, cSupportedPlugins[i]);
    Button* mResetPluginButton = new Button(NULL, mResetSocketCommand, "Reset");

    mRequirementsTable->addComponent(mTypeLabel,             TYPE_COLUMN,          i);
    mRequirementsTable->addComponent(mSelectedInstanceLabel, INSTANCE_COLUMN,      i);
    mRequirementsTable->addComponent(mChoosePluginButton,    CHOOSE_BUTTON_COLUMN, i);
    mRequirementsTable->addComponent(mResetPluginButton,     RESET_BUTTON_COLUMN,  i);
  }

  EdgeRelation* mAdjacentTitle = new EdgeRelation(mTitleLabel, EdgeRelation::OUTSIDE);
  IComponentBoundsCalculator* mTableLayout = new ComponentEdgeLayout(mAdjacentTitle, mInsideDialog, NULL, NULL, mRequirementsTable);
  mRequirementsTable->setBoundsCalculator(mTableLayout);

  addComponent(mTitleLabel);
  addComponent(mCloseButton);
  addComponent(mRequirementsTable);

  setFocusedComponent(mRequirementsTable);
}

std::string* PluginRequirementsComponent::getTitle(IPluginSupport* pluginSupport) {
  return new std::string(pluginSupport->getName() + " Plugins");
}

void PluginRequirementsComponent::updateResizableDialogContent(int milliseconds) {
  // Nothing to do.
}

void PluginRequirementsComponent::renderResizableDialogContent() {
  // Nothing to do.
}

bool PluginRequirementsComponent::inputResizableDialogContent(SDL_Event& event) {
  return false;
}

PluginRequirementsComponent::ResetSocketCommand::ResetSocketCommand(IPluginSupport* pluginSupport, PlugSocket* plugSocket) {
  cPluginSupport = pluginSupport;
  cPlugSocket = plugSocket;
}

void PluginRequirementsComponent::ResetSocketCommand::execute() {
  cPluginSupport->setPlugin(cPlugSocket, NULL);
}

PluginRequirementsComponent::CloseCommand::CloseCommand(PluginRequirementsComponent* parent) {
  cParent = parent;
}

void PluginRequirementsComponent::CloseCommand::execute() {
  cParent->close();
}
