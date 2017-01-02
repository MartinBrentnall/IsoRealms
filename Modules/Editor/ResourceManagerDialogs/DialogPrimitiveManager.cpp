/*
 * Copyright 2015 Martin Brentnall
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
#include "DialogPrimitiveManager.h"

DialogPrimitiveManager::DialogPrimitiveManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator, IEditingContext* editingContext) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogPrimitiveManager", resources) {
  projectManager->addProjectListener(this);
//  cAbsoluteColour = new Colour();
  cComponentBrowserBooleans = new ComponentResourceBrowser<IBoolean, BooleanIcon>(resources, resourceManager, projectManager, resourceLocator, editingContext, 0.0f);
  cComponentBrowserIntegers = new ComponentResourceBrowser<IInteger, NamedResourceIcon<IInteger>>(resources, resourceManager, projectManager, resourceLocator, editingContext, 0.0f);
  cComponentBrowserFloats   = new ComponentResourceBrowser<IFloat,   NamedResourceIcon<IFloat>>(  resources, resourceManager, projectManager, resourceLocator, editingContext, 0.0f);
  cComponentBrowserStrings  = new ComponentResourceBrowser<IString,  NamedResourceIcon<IString>>( resources, resourceManager, projectManager, resourceLocator, editingContext, 0.0f);
  addComponent("tabBoolean/containerBooleans", cComponentBrowserBooleans);
  addComponent("tabInteger/containerIntegers", cComponentBrowserIntegers);
  addComponent("tabFloat/containerFloats",     cComponentBrowserFloats);
  addComponent("tabString/containerStrings",   cComponentBrowserStrings);
  // TODO: Init resources
}

void DialogPrimitiveManager::addBooleanSelectionListener(IResourceSelectionListener<IBoolean>* listener) {
  cComponentBrowserBooleans->addResourceSelectionListener(listener);
  std::string mSelectedTab = getSelectedTab("tabbedContainer");
  setSelectedTab("tabbedContainer", "tabBoolean");
}

void DialogPrimitiveManager::addIntegerSelectionListener(IResourceSelectionListener<IInteger>* listener) {
  cComponentBrowserIntegers->addResourceSelectionListener(listener);
  std::string mSelectedTab = getSelectedTab("tabbedContainer");
  setSelectedTab("tabbedContainer", "tabInteger");
}

void DialogPrimitiveManager::addFloatSelectionListener(IResourceSelectionListener<IFloat>* listener) {
  cComponentBrowserFloats->addResourceSelectionListener(listener);
  std::string mSelectedTab = getSelectedTab("tabbedContainer");
  setSelectedTab("tabbedContainer", "tabFloat");
}

void DialogPrimitiveManager::addStringSelectionListener(IResourceSelectionListener<IString>* listener) {
  cComponentBrowserStrings->addResourceSelectionListener(listener);
  std::string mSelectedTab = getSelectedTab("tabbedContainer");
  setSelectedTab("tabbedContainer", "tabString");
}

void DialogPrimitiveManager::removeBooleanSelectionListener(IResourceSelectionListener<IBoolean>* listener) {
  cComponentBrowserBooleans->removeResourceSelectionListener(listener);
}

void DialogPrimitiveManager::removeIntegerSelectionListener(IResourceSelectionListener<IInteger>* listener) {
  cComponentBrowserIntegers->removeResourceSelectionListener(listener);
}

void DialogPrimitiveManager::removeFloatSelectionListener(IResourceSelectionListener<IFloat>* listener) {
  cComponentBrowserFloats->removeResourceSelectionListener(listener);
}

void DialogPrimitiveManager::removeStringSelectionListener(IResourceSelectionListener<IString>* listener) {
  cComponentBrowserStrings->removeResourceSelectionListener(listener);
}

void DialogPrimitiveManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<IBoolean*> mBooleans = mResourceManager->getAllBooleans();
  std::vector<IInteger*> mIntegers = mResourceManager->getAllIntegers();
  std::vector<IFloat*>   mFloats   = mResourceManager->getAllFloats();
  std::vector<IString*>  mStrings  = mResourceManager->getAllStrings();
  cComponentBrowserBooleans->initialise(mBooleans);
  cComponentBrowserIntegers->initialise(mIntegers);
  cComponentBrowserFloats->initialise(mFloats);
  cComponentBrowserStrings->initialise(mStrings);
}
