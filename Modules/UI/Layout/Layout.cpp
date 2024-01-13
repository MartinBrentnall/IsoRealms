/*
 * Copyright 2023 Martin Brentnall
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
#include "Layout.h"

#include "Editor/LayoutEditor.h"

namespace IsoRealms::UI {
  const std::string Layout::TAG_COMPONENT = "Component";

  const std::string Layout::ATTRIBUTE_NAME = "name";

  Layout::Layout(IProject* project, UI* ui) {
    // Nothing to do.
  }
  
  Layout::Layout(IProject* project, UI* ui, DOMNode& node, IOptions* options, IResourceData* data) :
            Layout(project, ui) {
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == TAG_COMPONENT) {
        std::string mComponentName = mNode.getAttribute(ATTRIBUTE_NAME);
        if (cComponentsByName.find(mComponentName) != cComponentsByName.end()) {
          throw ParseException("Duplicate component name in screen layout: " + mComponentName);
        }
        // TODO: Is this an implicit "new"!!?
        cComponentsByName.emplace(std::piecewise_construct, std::forward_as_tuple(mComponentName), std::forward_as_tuple(project, *this, mNode));
        cComponentsByOrder.emplace_back(&(cComponentsByName.find(mComponentName)->second));
      } else {
        throw ParseException("Unknown tag for UI/Layout: " + mChildName);
      }
    }
  }

  void Layout::registerAssets(IAssetRegistry* assets) {
    assets->add(static_cast<IEditable*>(this), "", "Screen Layouts");
    assets->add(static_cast<IScreen*>(this), "", "Screen Layouts");
    for (std::pair<const std::string, LayoutComponent>& mComponent : cComponentsByName) {
      mComponent.second.registerAssets(assets, mComponent.first);
    }
  }

  void Layout::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IEditable*>(this));
    assets->remove(static_cast<IScreen*>(this));
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      mComponent->unregisterAssets(assets);
    }
  }

  void Layout::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    for (const std::pair<const std::string, LayoutComponent>& mComponent : cComponentsByName) {
      if (mComponent.first != "[screen]") {
        DOMNodeWriter mComponentNode = node->addBranch(TAG_COMPONENT);
        mComponentNode.addAttribute(ATTRIBUTE_NAME, mComponent.first);
        mComponent.second.save(&mComponentNode, identifier);
      }
    }
  }

  void Layout::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Layout::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Layout::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void Layout::renderScreen(float scale, float aspectRatio) const {
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      mComponent->render(scale, aspectRatio);
    }
  }

  bool Layout::renderAssetIcon() const {
    return renderIcon();
  }

  IEditableScreen* Layout::createEditableScreen(IsoRealms::Project* project) {
//     std::unique_ptr<LayoutEditor> mScreen = std::make_unique<LayoutEditor>(this);
//     IEditableScreen* mReturnValue = mScreen.get();
//     cEditors[mReturnValue] = std::move(mScreen);
//     return mReturnValue;
    return nullptr;
  }

  void Layout::renderEditing(float aspectRatio) const {
    renderScreen(1.0f, aspectRatio);
  }

  LayoutComponent* Layout::pickComponent(float x, float y, float aspectRatio) const {
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      if (mComponent->contains(x, y, aspectRatio)) {
        return mComponent;
      }
    }
    return nullptr;
  }

  LayoutComponent* Layout::getComponent(const std::string& name) {
    std::map<std::string, LayoutComponent>::iterator mNamedComponent = cComponentsByName.find(name);
    if (mNamedComponent == cComponentsByName.end()) {
      throw ArgumentException("ERROR: Layout::getComponent: No component by specified name \"" + name + "\" in this layout.");
    }
    return &mNamedComponent->second;
  }

  std::string Layout::getName(LayoutComponent* component) {
    for (const std::pair<const std::string, LayoutComponent>& mComponent : cComponentsByName) {
      if (&mComponent.second == component) {
        return mComponent.first;
      }
    }
    throw ArgumentException("ERROR: Layout::getName: Specified component not found in this layout.");
  }

  void Layout::setName(LayoutComponent* component, const std::string& name) {

    // Check if another component already has the name.
    const LayoutComponent* mComponent = getComponent(name);
    if (mComponent == nullptr) {

      // The rename only takes place once we find the old entry so we can remove it.
      for (const std::pair<const std::string, LayoutComponent>& mNamedComponent : cComponentsByName) {
        if (&mNamedComponent.second == component) {
          mComponent = &mNamedComponent.second;
          cComponentsByName.erase(mNamedComponent.first);
//          cComponentsByName.emplace(name, *mComponent);
          return;
        }
      }
      throw ArgumentException("ERROR: Layout::setName: Specified component not found in this layout.");
    }
  }

  std::vector<std::string> Layout::getAvailableRelativeNames(LayoutComponent* component) {
    std::vector<std::string> mRelatableElementNames;
    for (const std::pair<const std::string, LayoutComponent>& mComponent : cComponentsByName) {
      if (!mComponent.second.isRelatedTo(component)) {
        mRelatableElementNames.push_back(mComponent.first);
      }
    }
    return mRelatableElementNames;
  }
}
