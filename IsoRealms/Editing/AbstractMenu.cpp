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
#include "AbstractMenu.h"

#include "Configurator.h"

namespace IsoRealms {
  AbstractMenu::AbstractMenu(Configurator* parent, IPropertyAppearance* appearance) :
          cParent(parent),
          cIntAppearance(appearance),
          cSelectedItem(0),
          cTitle("TODO"),
          cSelectionAnimation(0.0f),
          cAnimation(1.0f),
          cLeftHighlightAnimation(0.0f),
          cRightHighlightAnimation(0.0f),
          cLeftBoundaryAnimation(0.0f),
          cRightBoundaryAnimation(0.0f),
          cScroll(0.0f),
          cScrollAnimation(0.0f) {
  }

  void AbstractMenu::setTitle(const std::string& title) {
    cTitle = title;
  }

  void AbstractMenu::render() {
    if (getItemCount() > 0) {
      float mYLocation = (cParent->getTopIconPosition() - cSelectionAnimation * cIntAppearance->getLineHeight()) - cIntAppearance->getLineHeight() * 1.5f;

      glPushMatrix();
      if (cAnimation > 0.0f) {
        float mMenuPositionX = Utils::sine(0.0f, 2.0f, cAnimation);
        glTranslatef(mMenuPositionX, 0.0f, 0.0f);
      } else if (cAnimation < 0.0f) {
        float mMenuPositionX = Utils::sine(-2.0f, 0.0f, cAnimation + 1.0f);
        glTranslatef(mMenuPositionX, 0.0f, 0.0f);
      }

      if (cScrollAnimation >= 0.0f) {
        glTranslatef(0.0f, cScrollAnimation, 0.0f);
      }

      // Render selection shadow

      float mRightBoundary = -1.0f;
      for (unsigned int i = 0; i < getItemCount(); i++) {
        mRightBoundary = std::max(mRightBoundary, getRightSelectionBoundary(i));
      }

      glEnable(GL_BLEND);
      glColor4f(0.0f, 0.0f, 0.0f, 0.85f);
      Utils::renderRoundedRectangle(cParent->getScreenLeftBorder() - cIntAppearance->getSelectionHighlightHeight() * 0.5f,
                                   (cParent->getTopIconPosition() - (getItemCount() + 0.5f) * cIntAppearance->getLineHeight()) - cIntAppearance->getSelectionHighlightHeight(),
                                    mRightBoundary + cIntAppearance->getSelectionHighlightHeight(),
                                    cParent->getTopIconPosition() + cIntAppearance->getSelectionHighlightHeight(),
                                    cIntAppearance->getSelectionHighlightHeight() * 0.5f);
      // left bottom right top

      glColor4f(1.0f, 1.0f, 1.0f, 0.15f);
      Utils::renderBar(cLeftBoundaryAnimation, mYLocation - cIntAppearance->getSelectionHighlightHeight() * 0.5f, cRightBoundaryAnimation, mYLocation + cIntAppearance->getSelectionHighlightHeight() * 0.5f);
      LiteralColour(*cParent->getSelectionHighlight(), *cParent->getSelectionLocked(), cLockAnimation).set();
      Utils::renderBar(cLeftHighlightAnimation, mYLocation - cIntAppearance->getSelectionHighlightHeight() * 0.5f, cRightHighlightAnimation, mYLocation + cIntAppearance->getSelectionHighlightHeight() * 0.5f);

      // Render menu items
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      cIntAppearance->print(cParent->getScreenLeftBorder(), cParent->getTopIconPosition(), cIntAppearance->getScale(), IFont::Alignment::LEFT, cTitle.c_str());
      for (unsigned int i = 0; i < getItemCount(); ++i) {
        renderItem(i);
      }

      renderOverlay();
      glPopMatrix();
    }
  }

  void AbstractMenu::update(unsigned int milliseconds, bool active) {

    // Update screen animation.
    if (active) {
      if (cAnimation > 0.0f) {
        cAnimation = std::max(0.0f, cAnimation - 0.002f * milliseconds);
      } else {
        cAnimation = std::min(0.0f, cAnimation + 0.002f * milliseconds);
      }
    } else {
      cAnimation = std::max(-1.0f, cAnimation - 0.002f * milliseconds);
    }

    // Update scroll animation.
    if (cScrollAnimation != cScroll) {
      float mPotentialMovement = (cScroll - cScrollAnimation) * 0.15f;
      cScrollAnimation += mPotentialMovement;
    }

    // Update selection animation.
    if (cSelectionAnimation != cSelectedItem) {
      float mPotentialMovement = (cSelectedItem - cSelectionAnimation) * 0.15f;
      cSelectionAnimation += mPotentialMovement;
    }

    // Update left-right selection animation.
    if (getItemCount() > 0) {
      float mLeftBoundaryTarget   = getLeftSelectionBoundary(cSelectedItem);
      float mRightBoundaryTarget  = getRightSelectionBoundary(cSelectedItem);
      float mLeftHighlightTarget  = getLeftSelectionHighlight(cSelectedItem);
      float mRightHighlightTarget = getRightSelectionHighlight(cSelectedItem);
      if (cLeftBoundaryAnimation != mLeftBoundaryTarget) {
        float mPotentialMovement = (mLeftBoundaryTarget - cLeftBoundaryAnimation) * 0.15f;
        cLeftBoundaryAnimation += mPotentialMovement;
      }
      if (cRightBoundaryAnimation != mRightBoundaryTarget) {
        float mPotentialMovement = (mRightBoundaryTarget - cRightBoundaryAnimation) * 0.15f;
        cRightBoundaryAnimation += mPotentialMovement;
      }
      if (cRightHighlightAnimation != mRightHighlightTarget) {
        float mPotentialMovement = (mRightHighlightTarget - cRightHighlightAnimation) * 0.15f;
        cRightHighlightAnimation += mPotentialMovement;
      }
      if (cLeftHighlightAnimation != mLeftHighlightTarget) {
        float mPotentialMovement = (mLeftHighlightTarget - cLeftHighlightAnimation) * 0.15f;
        cLeftHighlightAnimation += mPotentialMovement;
      }
    }

    updateItems(milliseconds);

    if (isMenuInputLocked()) {
      cLockAnimation = std::min(1.0f, cLockAnimation + 0.005f * milliseconds);
    } else {
      cLockAnimation = std::max(0.0f, cLockAnimation - 0.005f * milliseconds);
    }
  }

  bool AbstractMenu::updateForClosure(unsigned int milliseconds) {
    cAnimation = std::min(1.0f, cAnimation + 0.002f * milliseconds);
    return cAnimation >= 1.0f;
  }

//   unsigned int AbstractMenu::getSelectedItem() {
//     return cSelectedItem;
//   }
//
//   void AbstractMenu::print(float x, float y, float scale, IFont::Alignment alignment, const std::string& text) {
//     return cParent->print(x, y, scale, alignment, text.c_str());
//   }
//
//   float AbstractMenu::getWidth(const std::string& text) const {
//     return cParent->getWidth(text.c_str());
//   }
//
  void AbstractMenu::selectNextMenuItem() {
    if (cSelectedItem < getItemCount() - 1) {
      cSelectedItem++;

      // Push scroll
      float mY = (cParent->getTopIconPosition() - cSelectedItem * cIntAppearance->getLineHeight()) - cIntAppearance->getSelectionHighlightHeight() * 0.5f;
      if (mY < cParent->getMenuBoundaryBottom() - cScroll) {
        float mMenuBottom = (cParent->getTopIconPosition() - (getItemCount() - 1) * cIntAppearance->getLineHeight()) - cIntAppearance->getSelectionHighlightHeight() * 0.5f;
        cScroll = std::min((cParent->getMenuBoundaryBottom() - mMenuBottom) - 0.15f, cParent->getMenuBoundaryBottom() - mY);
      }
    }
  }

  void AbstractMenu::selectPreviousMenuItem() {
    if (cSelectedItem > 0) {
      cSelectedItem--;

      // Push scroll
      float mY = (cParent->getTopIconPosition() - cSelectedItem * cIntAppearance->getLineHeight()) + cIntAppearance->getSelectionHighlightHeight() * 0.5f;
      if (mY > cParent->getMenuBoundaryTop() - cScroll) {
        cScroll = std::max(0.0f, cParent->getMenuBoundaryTop() - mY);
      }
    }
  }

  bool AbstractMenu::input(sf::Event& event) {
    if (input(cSelectedItem, event)) {
      return true;
    } else switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Down:   selectNextMenuItem();        break;
          case sf::Keyboard::Up:     selectPreviousMenuItem();    break;
          case sf::Keyboard::Escape: cParent->closeProjectMenu(); break;
          default:                                                break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 1: cParent->closeProjectMenu(); break;
        }
        break;
      }

      case sf::Event::JoystickMoved: {
// TODO        if (HatHandler::downPressed()) {selectNextMenuItem();}
//         if (HatHandler::upPressed())   {selectPreviousMenuItem();}
        break;
      }

      default: break;
    }
    return true;
  }

//   IFont* AbstractMenu::getFont() {
//     return cParent->getFont();
//   }
}

