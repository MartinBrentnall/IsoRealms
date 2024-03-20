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
#include "PropertyAsset.h"

namespace IsoRealms {
  PropertyAsset::PropertyAsset(IPropertyAppearance* appearance, const std::string& name, std::vector<std::pair<std::string, std::string>> items, std::vector<IAssetMenuItem*> globalMenuItems, std::function<bool(const std::string& item)> confirmationCallback) :
            Property(name),
            cAvailable(this, items, ""),
            cGlobalMenuItems(globalMenuItems),
            cSpecialEditor(nullptr),
            cIntAppearance(appearance),
            cConfirmationCallback(confirmationCallback) {
  }

  void PropertyAsset::setSelection(const std::string& item) {
    cSelection = getSelection(item);
  }
  
  void PropertyAsset::refreshItems(std::vector<std::pair<std::string, std::string>> items) {
    cAvailable.setItems(items);
  }
  
  void PropertyAsset::setSpecialItemSelected(int item) {
    cScroll = cIntAppearance->getY(0.0f) + item * cIntAppearance->getLineHeight();
  }
  
  bool PropertyAsset::updateEditing(unsigned int milliseconds) {
    cWidth.update(milliseconds);
    bool mClosed = cHeight.update(milliseconds);
    cSelectionWidth.update(milliseconds);
    cScroll.update(milliseconds);
    
    for (unsigned int i = 0; i < cSelection.size(); i++) {
      cSelection[i]->updateEditing(milliseconds, i == cSelection.size() - 1);
    }

    std::stack<unsigned int> mClosedIndices;
    for (unsigned int i = 0; i < cRuntimeSelectionClosed.size(); ++i) {
      if (cRuntimeSelectionClosed[i]->updateForClosure(milliseconds)) {
        mClosedIndices.push(i);
      }
    }
    
    while (!mClosedIndices.empty()) {
      unsigned int mClosedIndex = mClosedIndices.top();
      cRuntimeSelectionClosed.erase(cRuntimeSelectionClosed.begin() + mClosedIndex);
      mClosedIndices.pop();
    }
    
    if (cSpecialEditor != nullptr) {
      cSpecialEditor->update(milliseconds);
    }
    
    return mClosed;
  }
    
  void PropertyAsset::render(IPropertyAppearance* appearance) const {
    if (!cSelection.empty()) {
      cSelection.back()->render(appearance);
    }
  }

  void PropertyAsset::updateAnimationValues() {
    float mScale = cIntAppearance->getScale();
    float mItem = static_cast<float>(cSelection.back()->getIndex());
    cScroll = cIntAppearance->getY(0.0f) + mItem * (mScale * 2.5f); // TODO: 2.5f magic number bollocks
    cSelectionWidth = getWidth(cIntAppearance);
    cWidth = cSelection.back()->getMaxWidth(cIntAppearance);
    cHeight = (cSelection.back()->getItemCount() + cGlobalMenuItems.size()) * cIntAppearance->getLineHeight() - (cIntAppearance->getLineHeight() - cIntAppearance->getSelectionHighlightHeight());
  }
  
  std::vector<std::unique_ptr<PropertyAsset::ISelection>> PropertyAsset::getSelection(const std::string& id) const {
    for (unsigned int i = 0; i < cGlobalMenuItems.size(); i++) {
      std::vector<std::unique_ptr<ISelection>> mSelection = cGlobalMenuItems[i]->getSelection(id, &cAvailable, i); // TODO: PROBABLY SHOULDN'T PASS IN cAvailable HERE!
      if (!mSelection.empty()) {
        return mSelection;
      }
    }
    return cAvailable.getSelection(id);
  }
  
  void PropertyAsset::renderEditing(IPropertyAppearance* appearance) const {
    glBindTexture(GL_TEXTURE_2D, 0);

    float mY = appearance->getY(0.0f);
    float mHighlightHeight = cIntAppearance->getSelectionHighlightHeight();
    
    // Render black background.
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    Utils::renderCurve(0.0f,                cScroll.animation(),                                           mHighlightHeight * 0.5f, 0.0f,  0.25f);
    Utils::renderCurve(0.0f,               (cScroll.animation() + mHighlightHeight) - cHeight.animation(), mHighlightHeight * 0.5f, 0.25f, 0.5f);
    Utils::renderCurve(cWidth.animation(), (cScroll.animation() + mHighlightHeight) - cHeight.animation(), mHighlightHeight * 0.5f, 0.5f,  0.75f);
    Utils::renderCurve(cWidth.animation(),  cScroll.animation(),                                           mHighlightHeight * 0.5f, 0.75f, 1.0f);
    glBegin(GL_QUADS);
    Utils::renderRectangle(0.0f,                     (cScroll.animation() + mHighlightHeight * 0.5f) - cHeight.animation(), cWidth.animation(),                                          cScroll.animation() + mHighlightHeight * 0.5f);
    Utils::renderRectangle(mHighlightHeight * 0.5f,  (cScroll.animation() + mHighlightHeight)        - cHeight.animation(), 0.0f,                                                               cScroll.animation());
    Utils::renderRectangle(cWidth.animation(),       (cScroll.animation() + mHighlightHeight)        - cHeight.animation(), cWidth.animation() + mHighlightHeight * 0.5f, cScroll.animation());

    // Render cursor.
    glColor3f(1.0f, 0.0f, 0.2f);
    Utils::renderRectangle(0.0f, mY - mHighlightHeight * 0.5f, cSelectionWidth.animation(), mY + mHighlightHeight * 0.5f);
    glEnd();
    Utils::renderCurve(0.0f,                        mY, mHighlightHeight * 0.5f, 0.0f, 0.5f);
    Utils::renderCurve(cSelectionWidth.animation(), mY, mHighlightHeight * 0.5f, 0.5f, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Render line border.
    glBegin(GL_LINES);
    glVertex2f(0.0f,                                                         cScroll.animation() + mHighlightHeight * 0.5f);
    glVertex2f(cWidth.animation(),                                           cScroll.animation() + mHighlightHeight * 0.5f);
    glVertex2f(0.0f,                                                        (cScroll.animation() + mHighlightHeight * 0.5f) - cHeight.animation());
    glVertex2f(cWidth.animation(),                                          (cScroll.animation() + mHighlightHeight * 0.5f) - cHeight.animation());
    glVertex2f(                    -mHighlightHeight * 0.5f,  cScroll.animation());
    glVertex2f(                    -mHighlightHeight * 0.5f, (cScroll.animation() + mHighlightHeight)        - cHeight.animation());
    glVertex2f(cWidth.animation() + mHighlightHeight * 0.5f,  cScroll.animation());
    glVertex2f(cWidth.animation() + mHighlightHeight * 0.5f, (cScroll.animation() + mHighlightHeight)        - cHeight.animation());
    glEnd();
    Utils::renderCurveLine(0.0f,                cScroll.animation(),                                           mHighlightHeight * 0.5f, 0.0f,  0.25f);
    Utils::renderCurveLine(0.0f,               (cScroll.animation() + mHighlightHeight) - cHeight.animation(), mHighlightHeight * 0.5f, 0.25f, 0.5f);
    Utils::renderCurveLine(cWidth.animation(), (cScroll.animation() + mHighlightHeight) - cHeight.animation(), mHighlightHeight * 0.5f, 0.5f,  0.75f);
    Utils::renderCurveLine(cWidth.animation(),  cScroll.animation(),                                           mHighlightHeight * 0.5f, 0.75f, 1.0f);
    
//    IApplication* mApplication = project->getApplication();
//    ScreenArea mPreviousCrop = mApplication->crop(ScreenArea(0.0f, cWidth.animation(), (cScroll.animation() + mHighlightHeight * 0.5f) - cHeight.animation(), cScroll.animation() + mHighlightHeight * 0.5f));
    
    // TODO: Replace mYScroll with something that adjusts smoothly based on changing Selection
    float mYScroll = -cSelection.back()->getScroll();
    for (IAssetMenuItem* mMenuItem : cGlobalMenuItems) {
      mMenuItem->render(cIntAppearance, mYScroll);
      mYScroll += 1.0f;
    }
    
    for (const std::unique_ptr<ISelection>& mSelection : cSelection) {
      mSelection->renderEditing(cIntAppearance);
    }
    for (const std::unique_ptr<ISelection>& mSelection : cRuntimeSelectionClosed) {
      mSelection->renderEditing(cIntAppearance);
    }
//    mApplication->crop(mPreviousCrop);
    
    if (cSpecialEditor != nullptr) {
      cSpecialEditor->render(cIntAppearance);
    }
  }

  IEditable* PropertyAsset::edit(IAssetRegistry* assets) {
    cSelectionWidth.init(getWidth(cIntAppearance));
    cWidth.init(cSelectionWidth.value());
    cHeight.init(cIntAppearance->getScale() * 2.5f);
    cScroll.init(cIntAppearance->getY(0.0f));
    updateAnimationValues();
    return nullptr;
  }
  
  void PropertyAsset::close() {
    cSelectionWidth = getWidth(cIntAppearance);
    cWidth = cSelectionWidth.value();
    cHeight = cIntAppearance->getScale() * 2.5f;
    cScroll = cIntAppearance->getY(0.0f);
  }

  float PropertyAsset::getWidth(IPropertyAppearance* appearance) const {
    return cSpecialEditor != nullptr ? 0.0f : cSelection.back()->getSelectedWidth(appearance);
  }

  void PropertyAsset::moveCursorLeft() {
    if (cSelection.size() > 1) {
      cRuntimeSelectionClosed.emplace_back(std::move(cSelection.back()));
      cSelection.pop_back();
      updateAnimationValues();
    }
  }

  bool PropertyAsset::input(sf::Event& event) {
    if (cSpecialEditor != nullptr) {
      if (cSpecialEditor->input(event)) {
        cConfirmationCallback(cSpecialEditor->getValue());
        return true;
      }
    } else switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Escape:                   return true; // TODO: Restore original selection
          case sf::Keyboard::Left:   moveCursorLeft(); break;
          default:                                     break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 1: return true; // TODO: Restore original selection
        }
        break;
      }

      case sf::Event::JoystickMoved: {
// TODO        if (HatHandler::leftPressed()) {moveCursorLeft();}
        break;
      }

      default: break;
    }
    return cSelection.back()->input(event);
  }
  
  PropertyAsset::AssetMenu::Selection::Selection(const AssetMenu* const parent, int item, float transitionStart) :
            cParent(parent),
            cItem(item),
            cScrollAnimation(static_cast<float>(item)),
            cTransitionAnimation(transitionStart) {
  }
      
  unsigned int PropertyAsset::AssetMenu::Selection::getItemCount() const {
    return cParent->getItemCount();
  }

  unsigned int PropertyAsset::AssetMenu::Selection::getIndex() const {
    return cItem;
  }
  
  float PropertyAsset::AssetMenu::Selection::getScroll() const {
    return cScrollAnimation;
  }

  void PropertyAsset::AssetMenu::Selection::moveCursorUp() {
    if (cItem > 0) {
      cItem--;
      cParent->cParent->updateAnimationValues();
    }
  }

  void PropertyAsset::AssetMenu::Selection::moveCursorDown() {
    if (cItem < static_cast<unsigned int>(cParent->getItemCount() + cParent->cParent->cGlobalMenuItems.size() - 1)) {
      cItem++;
      cParent->cParent->updateAnimationValues();
    }
  }

  bool PropertyAsset::AssetMenu::Selection::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Up:     moveCursorUp();                                    break;
          case sf::Keyboard::Down:   moveCursorDown();                                  break;
          case sf::Keyboard::Return: if (getMenuItem()->executeAction()) {return true;} break;
          default:                                                                      break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: if (getMenuItem()->executeAction()) {return true;} break;
        }
        break;
      }

      case sf::Event::JoystickMoved: {
// TODO        if (HatHandler::downPressed()) {moveCursorDown();}
//         if (HatHandler::upPressed())   {moveCursorUp();}
        break;
      }

      default: break;
    }
    return false;
  }

  void PropertyAsset::AssetMenu::Selection::updateEditing(unsigned int milliseconds, bool active) {
    if (cScrollAnimation != cItem) {
      float mPotentialMovement = (static_cast<float>(cItem) - cScrollAnimation) * 0.15f;
      cScrollAnimation += mPotentialMovement;
    }
    
    // Update screen animation.
    if (active) {
      if (cTransitionAnimation > 0.0f) {
        cTransitionAnimation = std::max(0.0f, cTransitionAnimation - 0.005f * milliseconds);
      } else {
        cTransitionAnimation = std::min(0.0f, cTransitionAnimation + 0.005f * milliseconds);
      }
    } else {
      cTransitionAnimation = std::max(-1.0f, cTransitionAnimation - 0.005f * milliseconds);
    }
  }

  bool PropertyAsset::AssetMenu::Selection::updateForClosure(unsigned int milliseconds) {
    cTransitionAnimation = std::min(1.0f, cTransitionAnimation + 0.005f * milliseconds);
    return cTransitionAnimation >= 1.0f;
  }

  void PropertyAsset::AssetMenu::Selection::renderEditing(IPropertyAppearance* appearance) const {

    // Horizontal scrolling.
    if (cParent != nullptr) {
      glPushMatrix();
      if (cTransitionAnimation > 0.0f) {
        float mMenuPositionX = Utils::sine(0.0f, cParent->cParent->cWidth.animation(), cTransitionAnimation);
        glTranslatef(mMenuPositionX, 0.0f, 0.0f);
      } else if (cTransitionAnimation < 0.0f) {
        float mMenuPositionX = Utils::sine(-getMaxWidth(appearance), 0.0f, cTransitionAnimation + 1.0f);
        glTranslatef(mMenuPositionX, 0.0f, 0.0f);
      }
      
      // Render menu content.
      cParent->renderEditing(appearance, -cScrollAnimation);
      glPopMatrix();
    }
  }
  
  void PropertyAsset::AssetMenu::Selection::render(IPropertyAppearance* appearance) const {
    getMenuItem()->render(appearance, 0.0f);
  }
  
  float PropertyAsset::AssetMenu::Selection::getSelectedWidth(IPropertyAppearance* appearance) const {
    return getMenuItem()->getWidth(appearance);
  }
  
  float PropertyAsset::AssetMenu::Selection::getMaxWidth(IPropertyAppearance* appearance) const {
    return cParent->getMaxWidth(appearance);
  }
  
  PropertyAsset::IAssetMenuItem* PropertyAsset::AssetMenu::Selection::getMenuItem() const {
    return cItem < cParent->cParent->cGlobalMenuItems.size() ? cParent->cParent->cGlobalMenuItems[cItem] : cParent->cMenuItems[cItem - cParent->cParent->cGlobalMenuItems.size()].get();
  }
  
  PropertyAsset::AssetMenu::AssetMenu(PropertyAsset* parent, std::vector<std::pair<std::string, std::string>> ids, const std::string& prefix) :
            cParent(parent) {
    refresh(ids, prefix);
  }
  
  void PropertyAsset::AssetMenu::refresh(std::vector<std::pair<std::string, std::string>> ids, const std::string& prefix) {
    std::set<std::string> mPrefixes;
    for (std::pair<std::string, std::string> mID : ids) {
      std::string mFolderPath = mID.second;
      if (mFolderPath.substr(0, prefix.length() + 1) == prefix + "/" || prefix.empty()) {
        if (!prefix.empty()) {
          mFolderPath = mFolderPath.substr(prefix.length() + 1);
        }
        if (mFolderPath.find_first_of('/') == std::string::npos) {
          cMenuItems.emplace_back(std::make_unique<AssetMenuItemAsset>(cParent, mID.first, mFolderPath));
        } else {
          std::string mPrefix = mFolderPath.substr(0, mFolderPath.find_first_of('/'));
          if (!prefix.empty()) {
            mPrefix = prefix + "/" + mPrefix;
          }
          mPrefixes.insert(mPrefix);
        }
      }
    }
    
    for (std::string mPrefix : mPrefixes) {
      cMenuItems.emplace_back(std::make_unique<AssetMenuItemFolder>(cParent, std::make_unique<AssetMenu>(cParent, ids, mPrefix), mPrefix.substr(mPrefix.find_last_of('/') + 1)));
    }
  }
  
  void PropertyAsset::AssetMenu::setItems(std::vector<std::pair<std::string, std::string>> ids) {
    cMenuItems.clear();
    refresh(ids, "");
  }
  
  unsigned int PropertyAsset::AssetMenu::getItemCount() const {
    return static_cast<unsigned int>(cMenuItems.size());
  }  
  
  void IsoRealms::PropertyAsset::AssetMenu::renderEditing(IPropertyAppearance* appearance, float yOffset) const {
    int mLine = static_cast<int>(cParent->cGlobalMenuItems.size());
    for (const std::unique_ptr<IAssetMenuItem>& mMenuItem : cMenuItems) {
      mMenuItem->render(appearance, yOffset + mLine++);
    }
  }
  
  float PropertyAsset::AssetMenu::getMaxWidth(IPropertyAppearance* appearance) const {
    float mMaxWidth = 0.0f;
    for (IAssetMenuItem* mMenuItem : cParent->cGlobalMenuItems) {
      mMaxWidth = std::max(mMaxWidth, mMenuItem->getWidth(appearance));
    }
    for (const std::unique_ptr<IAssetMenuItem>& mMenuItem : cMenuItems) {
      mMaxWidth = std::max(mMaxWidth, mMenuItem->getWidth(appearance));
    }
    return mMaxWidth;
  }
  
  std::vector<std::unique_ptr<PropertyAsset::ISelection>> PropertyAsset::AssetMenu::getSelection(const std::string& id) const {
    for (unsigned int i = 0; i < cMenuItems.size(); i++) {
      std::vector<std::unique_ptr<ISelection>> mSelection = cMenuItems[i]->getSelection(id, this, i);
      if (!mSelection.empty()) {
        return mSelection;
      }
    }
    return std::vector<std::unique_ptr<ISelection>>();
  }
  
  PropertyAsset::AssetMenuItemFolder::AssetMenuItemFolder(PropertyAsset* parent, std::unique_ptr<AssetMenu> assetMenu, const std::string& label) :
          cParent(parent),
          cAssetMenu(std::move(assetMenu)),
          cLabel(label) {    
  }
  
  void PropertyAsset::AssetMenuItemFolder::render(IPropertyAppearance* appearance, float yOffset) const {
    glPushMatrix();
    appearance->transformForIcon(yOffset);
    Utils::renderIconBranch();
    glPopMatrix();
    appearance->print(cLabel, yOffset, appearance->getIconWidth() + appearance->getIconTextSpacing());
  }
  
  bool PropertyAsset::AssetMenuItemFolder::executeAction() {
    cParent->cSelection.emplace_back(std::make_unique<AssetMenu::Selection>(cAssetMenu.get(), static_cast<int>(cParent->cGlobalMenuItems.size()), 1.0f));
    cParent->updateAnimationValues();
    return false;
  }
  
  float PropertyAsset::AssetMenuItemFolder::getWidth(IPropertyAppearance* appearance) const {
    return appearance->getWidth(cLabel) + appearance->getIconWidth() + appearance->getIconTextSpacing();;
  }  
  
  std::vector<std::unique_ptr<PropertyAsset::ISelection>> PropertyAsset::AssetMenuItemFolder::getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const {
    std::vector<std::unique_ptr<PropertyAsset::ISelection>> mSelection = cAssetMenu->getSelection(id);
    if (!mSelection.empty()) {
      mSelection.emplace(mSelection.begin(), std::make_unique<AssetMenu::Selection>(menu, item + static_cast<int>(cParent->cGlobalMenuItems.size()), -1.0f));
    }
    return mSelection;
  }
  
  PropertyAsset::AssetMenuItemAsset::AssetMenuItemAsset(PropertyAsset* parent, const std::string assetID, const std::string& label) :
            cParent(parent),
            cAssetID(assetID),
            cLabel(label) {
  }
  
  void PropertyAsset::AssetMenuItemAsset::render(IPropertyAppearance* appearance, float yOffset) const {
    glPushMatrix();
    appearance->transformForIcon(yOffset);
    glEnable(GL_BLEND);
    if (!cParent->renderIcon(appearance, cAssetID)) {
      Utils::renderIconLeaf();
    }
    glPopMatrix();
    appearance->print(cLabel, yOffset, appearance->getIconWidth() + appearance->getIconTextSpacing());
  }
  
  bool PropertyAsset::AssetMenuItemAsset::executeAction() {
    return cParent->cConfirmationCallback(cAssetID);
//     cParent->cSelection.clear();
//     cParent->cSelection = cParent->getSelection(mConfirmedValue);
  }
  
  float PropertyAsset::AssetMenuItemAsset::getWidth(IPropertyAppearance* appearance) const {
    return appearance->getWidth(cLabel) + appearance->getIconWidth() + appearance->getIconTextSpacing();
  }

  std::vector<std::unique_ptr<PropertyAsset::ISelection>> PropertyAsset::AssetMenuItemAsset::getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const {
    std::vector<std::unique_ptr<PropertyAsset::ISelection>> mSelection;
    if (id == cAssetID) {
      mSelection.emplace_back(std::make_unique<AssetMenu::Selection>(menu, item + static_cast<int>(cParent->cGlobalMenuItems.size()), 0.0f));
    }
    return mSelection;
  }
}
