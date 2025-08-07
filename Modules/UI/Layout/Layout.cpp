/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Layout.h"

#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  const std::string Layout::JSON_COMPONENTS = "components";
  const std::string Layout::JSON_ID         = "id";

  Layout::Layout(IProject& project, UI& ui, IResourceData& data) :
            cResourceData(data),
            cUI(ui) {
  }
  
  Layout::Layout(IProject& project, UI& ui, IResourceData& data, JSONObject object) :
            Layout(project, ui, data) {
    for (JSONObject mComponentObject : object.getArray(JSON_COMPONENTS)) {
      std::string mComponentName = mComponentObject.getString(JSON_ID);
      if (cComponentsByName.find(mComponentName) != cComponentsByName.end()) {
        throw ParseException("Duplicate component name in screen layout: " + mComponentName);
      }
      // TODO: Is this an implicit "new"!!?
      cComponentsByName.emplace(std::piecewise_construct, std::forward_as_tuple(mComponentName), std::forward_as_tuple(project, *this, mComponentObject));
      cComponentsByOrder.emplace_back(&(cComponentsByName.find(mComponentName)->second));
    }
  }

  void Layout::registerAssets(ResourceAssetRegistry& assets) {
    assets.add(static_cast<IEditable*>(this), "", "Screen Layouts");
    assets.add(static_cast<IScreen*>(this), "", "Screen Layouts");
    for (std::pair<const std::string, LayoutComponent>& mComponent : cComponentsByName) {
      mComponent.second.registerAssets(assets, mComponent.first);
    }
  }

  void Layout::save(JSONObject object) const {
    JSONArray mComponentsArray = object.addArray(JSON_COMPONENTS);
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      JSONObject mComponentObject = mComponentsArray.addObject();
      std::string mComponentName = getName(mComponent);
      mComponentObject.addString(JSON_ID, mComponentName);
      mComponent->save(mComponentObject);
    }
  }

  void Layout::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Layout::renderIcon() const {
    return false;
  }

  void Layout::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyEditor(metadata.getPropertyData("Content"), this);
  }

  void Layout::updateEditing(unsigned int milliseconds) {
    for (const std::pair<IEditableScreen* const, std::unique_ptr<LayoutEditor>>& mEditor : cEditors) {
      mEditor.second->updateScreen(milliseconds);
    }
  }
  
  void Layout::reset() {
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      mComponent->reset();
    }
  }
  
  void Layout::renderScreen(float scale, float aspectRatio) const {
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      mComponent->render(scale, aspectRatio);
    }
  }

  bool Layout::renderAssetIcon() const {
    return renderIcon();
  }

  void Layout::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Layout::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool Layout::isDefaultConfiguration() const {
    return true;
  }

  IEditableScreen* Layout::createEditableScreen(IsoRealms::Project* project, IDialogManager& dialogManager) {
    std::unique_ptr<LayoutEditor> mScreen = std::make_unique<LayoutEditor>(*this, dialogManager);
    IEditableScreen* mReturnValue = mScreen.get();
    cEditors[mReturnValue] = std::move(mScreen);
    return mReturnValue;
  }

  void Layout::renderEditing(float aspectRatio) const {
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      mComponent->renderEditor(1.0f, aspectRatio);
    }
  }

  LayoutComponent* Layout::pickComponent(float x, float y, float aspectRatio) const {
    for (LayoutComponent* mComponent : std::ranges::views::reverse(cComponentsByOrder)) {
      if (mComponent->contains(x, y, aspectRatio)) {
        return mComponent;
      }
    }
    return nullptr;
  }

  LayoutComponent* Layout::pickPreviousComponent(float x, float y, float aspectRatio, LayoutComponent* current) const {
    if (current == nullptr) {
      return pickComponent(x, y, aspectRatio);
    }
    LayoutComponent* mFirstComponent = nullptr;
    bool mMatchedCurrent = false;
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      if (mComponent->contains(x, y, aspectRatio)) {
        if (mMatchedCurrent) {
          return mComponent;
        }
        if (mComponent == current) {
          mMatchedCurrent = true;
        }
        if (mFirstComponent == nullptr) {
          mFirstComponent = mComponent;
        }
      }
    }
    return mFirstComponent;
  }

  LayoutComponent* Layout::pickNextComponent(float x, float y, float aspectRatio, LayoutComponent* current) const {
    LayoutComponent* mFirstComponent = nullptr;
    bool mMatchedCurrent = false;
    for (LayoutComponent* mComponent : std::ranges::views::reverse(cComponentsByOrder)) {
      if (mComponent->contains(x, y, aspectRatio)) {
        if (mMatchedCurrent) {
          return mComponent;
        }
        if (mComponent == current) {
          mMatchedCurrent = true;
        }
        if (mFirstComponent == nullptr) {
          mFirstComponent = mComponent;
        }
      }
    }
    return mFirstComponent;
  }

  UI& Layout::getUI() const {
    return cUI;
  }

  IResourceData& Layout::getResourceData() {
    return cResourceData;
  }

  LayoutComponent* Layout::createComponent(float x1, float y1, float x2, float y2, float aspectRatio) {
    std::string mComponentName = Utils::getAvailableKey(cComponentsByName, "New Component");
    cComponentsByName.emplace(std::piecewise_construct, std::forward_as_tuple(mComponentName), std::forward_as_tuple(cUI.getProject(), *this, x1, y1, x2, y2, aspectRatio));
    return cComponentsByOrder.emplace_back(&(cComponentsByName.find(mComponentName)->second));
  }
  
  LayoutComponent* Layout::createComponent(JSONObject& object) {
    std::string mComponentName = Utils::getAvailableKey(cComponentsByName, "New Component");
    cComponentsByName.emplace(std::piecewise_construct, std::forward_as_tuple(mComponentName), std::forward_as_tuple(cUI.getProject(), *this, object));
    return cComponentsByOrder.emplace_back(&(cComponentsByName.find(mComponentName)->second));
  }

  void Layout::deleteComponent(LayoutComponent* component) {
    Utils::removeElement(cComponentsByOrder, component);
    cComponentsByName.erase(getName(component));
  }
  
  void Layout::moveComponentBackward(LayoutComponent* component) {
    int mOldIndex = getIndex(component);
    if (mOldIndex > 0) {
      cComponentsByOrder.erase(cComponentsByOrder.begin() + mOldIndex);
      cComponentsByOrder.insert(cComponentsByOrder.begin() + (mOldIndex - 1), component);
    }
  }
  
  void Layout::moveComponentForward(LayoutComponent* component) {
    int mOldIndex = getIndex(component);
    if (mOldIndex < static_cast<int>(cComponentsByOrder.size()) - 1) {
      cComponentsByOrder.erase(cComponentsByOrder.begin() + mOldIndex);
      cComponentsByOrder.insert(cComponentsByOrder.begin() + mOldIndex + 1, component);
    }
  }
  
  void Layout::moveComponentToBack(LayoutComponent* component) {
    Utils::removeElement(cComponentsByOrder, component);
    cComponentsByOrder.insert(cComponentsByOrder.begin(), component);
  }
  
  void Layout::moveComponentToFront(LayoutComponent* component) {
    Utils::removeElement(cComponentsByOrder, component);
    cComponentsByOrder.emplace_back(component);
  }
  
  LayoutComponent* Layout::getComponent(const std::string& name) {
    std::map<std::string, LayoutComponent>::iterator mNamedComponent = cComponentsByName.find(name);
    if (mNamedComponent == cComponentsByName.end()) {
      throw ArgumentException("ERROR: Layout::getComponent: No component by specified name \"" + name + "\" in this layout.");
    }
    return &mNamedComponent->second;
  }

  std::string Layout::getName(const LayoutComponent* component) const {
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
  
  int Layout::getIndex(LayoutComponent* component) const {
    for (unsigned int i = 0; i < cComponentsByOrder.size(); i++) {
      if (cComponentsByOrder[i] == component) {
        return i;
      }
    }
    return -1; // TODO: Throw.
  }
}
