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

PluginRequirementsComponent::PluginRequirementsComponent(IComponentContainer* componentContainer, IPluginRegistryAccessor* pluginRegistryAccessor, IPluginSupport* pluginSupport, float x, float y) : ResizableDialog(componentContainer, getTitle(pluginSupport), x, y, 1.0f, 1.0f) {
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

  // Put a scrollable container below label and above close button
  EdgeRelation* mAdjacentTitle = new EdgeRelation(mTitleLabel, EdgeRelation::OUTSIDE);
  EdgeRelation* mAdjacentButton = new EdgeRelation(mCloseButton, EdgeRelation::OUTSIDE);
  IComponentBoundsCalculator* mTableLayout = new ComponentEdgeLayout(mAdjacentTitle, mInsideDialog, mAdjacentButton, mInsideDialog, NULL);
  ScrollableContainer* mScrollableContainer = new ScrollableContainer();
  mScrollableContainer->setBoundsCalculator(mTableLayout);

  // Put table inside the scrollable container
  FlexibleGridLayoutComponent* mRequirementsTable = new FlexibleGridLayoutComponent(4, cSupportedPlugins.size() + 1);

  TextLabelComponent* mTypeHeading = new TextLabelComponent("Type");
  TextLabelComponent* mSelectedHeading = new TextLabelComponent("Selected");
  mRequirementsTable->addComponent(mTypeHeading,     TYPE_COLUMN,     cSupportedPlugins.size());
  mRequirementsTable->addComponent(mSelectedHeading, INSTANCE_COLUMN, cSupportedPlugins.size());
  PluginRegistry* mPluginRegistry = pluginRegistryAccessor->getPluginRegistry();
  for (unsigned int i = 0; i < cSupportedPlugins.size(); i++) {
    std::string mPluginType = cSupportedPlugins[i]->getType();
    TextLabelComponent* mTypeLabel = new TextLabelComponent(mPluginType);
    IPlugin* mPlugin = cPluginSupport->getClientPlugin(cSupportedPlugins[i]);
    std::string mInstanceName = mPlugin != NULL ? mPluginRegistry->getInstanceName(mPluginType, mPlugin) : "<none>";
    TextLabelComponent* mSelectedInstanceLabel = new TextLabelComponent(mInstanceName);
    ICommand* mChoosePluginInstanceCommand = new ChoosePluginImplementationCommand(cPluginSupport, cSupportedPlugins[i], componentContainer, pluginRegistryAccessor, mPluginType);
    Button* mChoosePluginButton = new Button(NULL, mChoosePluginInstanceCommand, "Choose...");
    ICommand* mResetSocketCommand = new ResetSocketCommand(cPluginSupport, cSupportedPlugins[i]);
    Button* mResetPluginButton = new Button(NULL, mResetSocketCommand, "Reset");

    mRequirementsTable->addComponent(mTypeLabel,             TYPE_COLUMN,          i);
    mRequirementsTable->addComponent(mSelectedInstanceLabel, INSTANCE_COLUMN,      i);
    mRequirementsTable->addComponent(mChoosePluginButton,    CHOOSE_BUTTON_COLUMN, i);
    mRequirementsTable->addComponent(mResetPluginButton,     RESET_BUTTON_COLUMN,  i);
  }
  mScrollableContainer->setRootComponent(mRequirementsTable);

  addComponent(mTitleLabel);
  addComponent(mCloseButton);
  addComponent(mScrollableContainer);

  setFocusedComponent(mRequirementsTable);
}

std::string PluginRequirementsComponent::getTitle(IPluginSupport* pluginSupport) {
  return pluginSupport->getName() + " Plugins";
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
