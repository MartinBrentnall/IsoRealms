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

#include "IsoRealms/Project/Options.h"

#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  Layout::Layout(UI& ui, IComponentData& data) :
            cComponentData(data),
            cUI(ui) {
  }
  
  void Layout::load(IComponentData& resourceData, JSONObject object) {
    // Nothing to do.
  }

  void Layout::save(IComponentData& resourceData, JSONObject object) const {
    // Nothing to do.
  }

  void Layout::define(IComponentDefiner& definer) {
    definer.propertyEditor("Content", this);
    definer.array(JSON_COMPONENTS, cComponentsByOrder, [](LayoutComponent* mComponent) -> LayoutComponent& {return *mComponent;}, [this, &definer](LayoutComponent& component) {
      Options mComponentsHint;
      mComponentsHint.addOption(Options::PROPERTY_NO_EDIT, "true");
      definer.scope("Component", getName(&component), [this, &component, &definer](IComponentDefiner& editingDefiner) {
        if (editingDefiner.loadsPersistedValues() || editingDefiner.savesPersistedValues()) {
          component.define(editingDefiner);
        }
      }, nullptr, mComponentsHint);
    }, [this]() -> LayoutComponent& {
      return *createComponent(0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    });
  }

  void Layout::publish(ResourcePublisher& publisher) {
    publisher.publish(static_cast<IEditable*>(this), "", "Screen Layouts");
    publisher.publish(static_cast<IScreen*>(this), "", "Screen Layouts");
    for (std::pair<const std::string, LayoutComponent>& mComponent : cComponentsByName) {
      mComponent.second.publish(publisher, mComponent.first);
    }
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

  IEditableScreen* Layout::createEditableScreen(IsoRealms::Project* project, IDialogManager& dialogManager) {
    std::unique_ptr<LayoutEditor> mScreen = std::make_unique<LayoutEditor>(*this, dialogManager);
    IEditableScreen* mReturnValue = mScreen.get();
    cEditors[mReturnValue] = std::move(mScreen);
    return mReturnValue;
  }

  void Layout::renderEditing(float scale, float aspectRatio) const {
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      mComponent->renderRegion(scale, aspectRatio);
    }
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      mComponent->renderEditor(1.0f, aspectRatio);
    }
  }

  LayoutComponent* Layout::pickComponent(float x, float y, float scale, float aspectRatio) const {
    for (LayoutComponent* mComponent : std::ranges::views::reverse(cComponentsByOrder)) {
      if (mComponent->contains(x, y, scale, aspectRatio)) {
        return mComponent;
      }
    }
    return nullptr;
  }

  LayoutComponent* Layout::pickPreviousComponent(float x, float y, float scale, float aspectRatio, LayoutComponent* current) const {
    if (current == nullptr) {
      return pickComponent(x, y, scale, aspectRatio);
    }
    LayoutComponent* mFirstComponent = nullptr;
    bool mMatchedCurrent = false;
    for (LayoutComponent* mComponent : cComponentsByOrder) {
      if (mComponent->contains(x, y, scale, aspectRatio)) {
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

  LayoutComponent* Layout::pickNextComponent(float x, float y, float scale, float aspectRatio, LayoutComponent* current) const {
    LayoutComponent* mFirstComponent = nullptr;
    bool mMatchedCurrent = false;
    for (LayoutComponent* mComponent : std::ranges::views::reverse(cComponentsByOrder)) {
      if (mComponent->contains(x, y, scale, aspectRatio)) {
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

  IComponentData& Layout::getComponentData() {
    return cComponentData;
  }

  LayoutComponent* Layout::createComponent(float x1, float y1, float x2, float y2, float aspectRatio) {
    std::string mComponentName = Utils::getAvailableKey(cComponentsByName, "New Component");
    cComponentsByName.emplace(std::piecewise_construct, std::forward_as_tuple(mComponentName), std::forward_as_tuple(*this, x1, y1, x2, y2, aspectRatio));
    return cComponentsByOrder.emplace_back(&(cComponentsByName.find(mComponentName)->second));
  }

  LayoutComponent* Layout::createComponent(JSONObject& object) {
    std::string mComponentName = Utils::getAvailableKey(cComponentsByName, "New Component");
    cComponentsByName.emplace(std::piecewise_construct, std::forward_as_tuple(mComponentName), std::forward_as_tuple(*this, object));
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
    std::map<std::string, LayoutComponent>::iterator mExisting = cComponentsByName.find(name);
    if (mExisting != cComponentsByName.end() && &mExisting->second != component) {
      throw ArgumentException("ERROR: Layout::setName: Duplicate component name \"" + name + "\".");
    }
    
    for (std::map<std::string, LayoutComponent>::iterator mNamedComponent = cComponentsByName.begin(); mNamedComponent != cComponentsByName.end(); ++mNamedComponent) {
      if (&mNamedComponent->second == component) {
        if (mNamedComponent->first != name) {
          std::map<std::string, LayoutComponent>::node_type mNode = cComponentsByName.extract(mNamedComponent);
          mNode.key() = name;
          cComponentsByName.insert(std::move(mNode));
        }
        return;
      }//
    }
    throw ArgumentException("ERROR: Layout::setName: Specified component not found in this layout.");
  }

  bool Layout::isNameAllowed(LayoutComponent* component, const std::string& name) {
    std::map<std::string, LayoutComponent>::iterator mNamedComponent = cComponentsByName.find(name);
    if (mNamedComponent != cComponentsByName.end()) {
      return &mNamedComponent->second == component;
    }
    return true;
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
