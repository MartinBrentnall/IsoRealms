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
#include "PropertyCode.h"

#include "IsoRealms/Common/ScreenArea.h"
#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/IApplication.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Utils.h"

#include "IPropertyManager.h"

namespace IsoRealms {
  PropertyCode::PropertyCode(IProject& project, const PropertyData& data, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) :
            Property(data, removeFunction),
            cProject(project),
            cGetter(getter),
            cSetter(setter) {
  }

  void PropertyCode::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    glPushMatrix();
    glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    Utils::renderIconCustom();
    glPopMatrix();
    mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, "Edit...");
  }

  float PropertyCode::getValueWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, "Edit...") + mFontSize * 2.25f;
  }

  void PropertyCode::confirm(IPropertyManager& manager, float y) {
    // IUIStyle& mStyle = manager.getPropertyStyle();
    // IFont* mFont = mStyle.getFont();
    // float mFontSize = mStyle.getFontSize();
    manager.edit(std::make_unique<Editor>(*this));
  }

  bool PropertyCode::hasConfiguration() const {
    return false;
  }
  
  void PropertyCode::configure(IPropertyManager& manager) {
    // Nothing to do.
  }
  
  PropertyCode::Editor::Editor(PropertyCode& parent) :
            cParent(parent),
            cEditingCode(parent.cGetter()),
            cCaret(0),
            cMouseSelecting(false),
            cLineStartIndex(0),
            cLine(0),
            cCaretXPosition(0.0f),
            cSelection(0),
            cCaretOffsetX(0),
            cScrollX(0.0f), 
            cScrollY(0.0f),
            cOpenness(0),
            cClosing(false) {
  }

  void PropertyCode::Editor::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getCodeFont();
    float mFontSize = style.getCodeFontSize();

    float mTotalCodeWidth = mFont->getWidth(mFontSize, cEditingCode);
    float mTotalCodeHeight = mFont->getHeight(mFontSize, cEditingCode);

    float mOpenWidth  = std::min(mTotalCodeWidth + mFontSize * 4.0f, aspectRatio * 2.0f - mFontSize * 2.0f);
    float mOpenHeight = std::min(mTotalCodeHeight + mFontSize * 4.0f, 2.0f - mFontSize * 2.0f);
    float mOpenLeft   = -mOpenWidth  / 2.0f;
    float mOpenRight  =  mOpenWidth  / 2.0f;
    float mOpenBottom = -mOpenHeight / 2.0f;
    float mOpenTop    =  mOpenHeight / 2.0f;

    float mUIFontSize = style.getFontSize();

    float mClosedLeft   = x - mUIFontSize * 2.0f;
    float mClosedRight  = x + cParent.getValueWidth(style) + mUIFontSize * 2.0f;
    float mClosedBottom = y;
    float mClosedTop    = y + mUIFontSize * 2.0f;

    float mFrameLeft   = mClosedLeft   + (mOpenLeft   - mClosedLeft)   * (cOpenness / 250.0f);
    float mFrameRight  = mClosedRight  + (mOpenRight  - mClosedRight)  * (cOpenness / 250.0f);
    float mFrameBottom = mClosedBottom + (mOpenBottom - mClosedBottom) * (cOpenness / 250.0f);
    float mFrameTop    = mClosedTop    + (mOpenTop    - mClosedTop)    * (cOpenness / 250.0f);

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    Utils::renderRoundedRectangle(mFrameLeft, mFrameBottom, mFrameRight, mFrameTop, mFontSize * 2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    Utils::renderRoundedRectangleLines(mFrameLeft, mFrameBottom, mFrameRight, mFrameTop, mFontSize * 2.0f);

    float mLineHeight = mFont->getHeight(mFontSize, "A");

    IApplication& mApplication = style.getProject().getApplication();
    ScreenArea mPreviousCrop = mApplication.crop(ScreenArea(mFrameLeft + mFontSize, mFrameRight - mFontSize, mFrameBottom + mFontSize, mFrameTop - mFontSize));
    
    glPushMatrix();
    glTranslatef(cScrollX.animation(), cScrollY.animation(), 0.0f);

    // Render selection highlight.
    if (cCaret != cSelection) {
      unsigned int mSelectionStart = std::min(cCaret, cSelection);
      unsigned int mSelectionEnd = std::max(cCaret, cSelection);

      unsigned int mCurrentLineNumber = 0;
      for (unsigned int i = 0; i < mSelectionStart; i++) {
        if (cEditingCode[i] == '\n') {
          mCurrentLineNumber++;
        }
      }

      unsigned int mCurrentIndex = mSelectionStart;
      while (mCurrentIndex < mSelectionEnd) {
        std::string::size_type mCurrentLineStart = cEditingCode.substr(0, mCurrentIndex).find_last_of('\n');
        std::string::size_type mCurrentLineEnd = cEditingCode.substr(mCurrentIndex).find_first_of('\n');

        mCurrentLineStart = mCurrentLineStart == std::string::npos ? 0 : mCurrentLineStart + 1;
        mCurrentLineEnd = mCurrentLineEnd == std::string::npos ? cEditingCode.length() : mCurrentLineEnd + mCurrentIndex;

        std::string mCurrentLineText = cEditingCode.substr(mCurrentLineStart, mCurrentLineEnd - mCurrentLineStart);

        unsigned int mLineHighlightLeft = std::max(mSelectionStart, static_cast<unsigned int>(mCurrentLineStart));
        unsigned int mLineHighlightRight = std::min(mSelectionEnd, static_cast<unsigned int>(mCurrentLineEnd));

        float mHighlightLeft = mFrameLeft + mFontSize * 2.0f + mFont->getWidth(mFontSize, mCurrentLineText.substr(0, mLineHighlightLeft - mCurrentLineStart));
        float mHighlightRight = mFrameLeft + mFontSize * 2.0f + mFont->getWidth(mFontSize, mCurrentLineText.substr(0, mLineHighlightRight - mCurrentLineStart));

        glColor3f(1.0f, 0.0f, 0.5f);
        glBegin(GL_QUADS);
        glVertex2f(mHighlightLeft,  mFrameTop - (mFontSize * 2.0f + mCurrentLineNumber * mLineHeight));
        glVertex2f(mHighlightLeft,  mFrameTop - (mFontSize * 4.5f + mCurrentLineNumber * mLineHeight));
        glVertex2f(mHighlightRight, mFrameTop - (mFontSize * 4.5f + mCurrentLineNumber * mLineHeight));
        glVertex2f(mHighlightRight, mFrameTop - (mFontSize * 2.0f + mCurrentLineNumber * mLineHeight));
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        mCurrentIndex = mCurrentLineEnd + 1;
        mCurrentLineNumber++;
      }
    }

    // Render the actual code.
    mFont->print(mFrameLeft + mFontSize * 2.0f, mFrameTop - mFontSize * 4.0f, mFontSize, IFont::Alignment::LEFT, cEditingCode);

    // Render the caret.
    if (cBlinkShowing) {
      glBindTexture(GL_TEXTURE_2D, 0);
      glLineWidth(2.0);
      float mCaretX = mFrameLeft + mFontSize * 2.0f + cCaretOffsetX;
      glColor3f(1.0f, 1.0f, 1.0f);
      glBegin(GL_LINES);
      glVertex2f(mCaretX, mFrameTop - (mFontSize * 4.5f + cLine * mLineHeight));
      glVertex2f(mCaretX, mFrameTop - (mFontSize * 2.0f + cLine * mLineHeight));
      glEnd();
    }
    
    glPopMatrix();
    mApplication.crop(mPreviousCrop);
    
    // Render confirmation choice.
    if (cConfirmationSelection != nullptr) {
      cConfirmationSelection->render(style);
    }
    if (cClosedConfirmationSelection != nullptr) {
      cClosedConfirmationSelection->render(style);
    }
  }

  bool PropertyCode::Editor::update(unsigned int milliseconds) {
    cScrollX.update(milliseconds);
    cScrollY.update(milliseconds);
    cDelayUntilBlinkChange -= milliseconds;
    if (cDelayUntilBlinkChange <= 0) {
      cDelayUntilBlinkChange += BLINK_DELAY;
      cBlinkShowing = !cBlinkShowing;
    }
    
    if (cClosing) {
      cOpenness = std::max(cOpenness - static_cast<int>(milliseconds), 0);
    } else if (cOpenness < 250) {
      cOpenness = std::min(cOpenness + static_cast<int>(milliseconds), 250);
    }
    
    if (cConfirmationSelection != nullptr) {
      cConfirmationSelection->update(milliseconds);
    }
    if (cClosedConfirmationSelection != nullptr) {
      if (cClosedConfirmationSelection->update(milliseconds)) {
        cClosedConfirmationSelection = nullptr;
      }
    }
    return cOpenness == 0;
  }

  bool PropertyCode::Editor::input(UISignalID id, IUIStyle& style) {
    if (cConfirmationSelection != nullptr) {
      return cConfirmationSelection->input(id);
    }
    return false;
  }

  bool PropertyCode::Editor::input(sf::Event& event, IUIStyle& style) {
    if (cConfirmationSelection != nullptr) {
      // Nothing to do.
    } else switch (event.type) {
      case sf::Event::TextEntered: {
        if (static_cast<int>(event.text.unicode) >= 32 && static_cast<int>(event.text.unicode) <= 126) {
          clearSelection(style);

          cUndoStack.emplace(Edit(*this, true, std::string{static_cast<char>(event.text.unicode)}, cCaret));
          while (!cRedoStack.empty()) {cRedoStack.pop();}

          cEditingCode = cEditingCode.substr(0, cCaret) + static_cast<char>(event.text.unicode) + cEditingCode.substr(cCaret);
          cCaret++;
          updateValues(style);
          updateCaretColumnPosition(style);
          cSelection = cCaret;
        }
        break;
      }

      case sf::Event::MouseButtonPressed: {
        switch (event.mouseButton.button) {
          case sf::Mouse::Left: {
            cMouseSelecting = true;
            cCaret = getCaretPosition(style, event.mouseButton.x, event.mouseButton.y);
            cSelection = cCaret;
            updateValues(style);
            break;
          }

          default: {
            break;
          }
        }
        break;
      }

      case sf::Event::MouseButtonReleased: {
        switch (event.mouseButton.button) {
          case sf::Mouse::Left: {
            cMouseSelecting = false;
            break;
          }

          default: {
            break;
          }
        }
        break;
      }

      case sf::Event::MouseMoved: {
        if (cMouseSelecting) {
          cCaret = getCaretPosition(style, event.mouseMove.x, event.mouseMove.y);
          updateValues(style);
        }
        break;
      }

      case sf::Event::MouseWheelScrolled: {
        switch (event.mouseWheelScroll.wheel) {
          case sf::Mouse::VerticalWheel: {
            IFont* mFont = style.getCodeFont();
            float mFontSize = style.getCodeFontSize();
            float mLineHeight = mFont->getHeight(mFontSize, "A");
            float mContentHeight = mFont->getHeight(mFontSize, cEditingCode);
            float mAvailableHeight = (2.0f - mFontSize * 4.0f) - mLineHeight;
            float mMaxScroll = std::max(0.0f, mContentHeight - mAvailableHeight);
            cScrollY = std::min(mMaxScroll, std::max(0.0f, cScrollY.value() - event.mouseWheelScroll.delta * mLineHeight * 3.0f));
            break;
          }

          default: {
            break;
          }
        }
        break;
      }

      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Right: {
            if (cCaret < cEditingCode.length()) {
              cCaret++;
              updateValues(style);
              updateCaretColumnPosition(style);
            }

            if (!event.key.shift) {
              cSelection = cCaret;
            }
            break;
          }

          case sf::Keyboard::Left: {
            if (cCaret > 0) {
              cCaret--;
              updateValues(style);
              updateCaretColumnPosition(style);
            }

            if (!event.key.shift) {
              cSelection = cCaret;
            }
            break;
          }

          case sf::Keyboard::Up: {
            if (cLine == 0) {
              cCaret = 0;
            } else {
              IFont* mFont = style.getCodeFont();
              float mFontSize = style.getCodeFontSize();
              std::string::size_type mPreviousLineStartIndex = cEditingCode.substr(0, cLineStartIndex - 1).find_last_of('\n');
              if (mPreviousLineStartIndex == std::string::npos) {
                mPreviousLineStartIndex = 0;
              }
              std::string mPreviousLine = cEditingCode.substr(mPreviousLineStartIndex, (cLineStartIndex - 1) - mPreviousLineStartIndex);
              unsigned int mCharPosition = mFont->getChar(cCaretXPosition, mFontSize, mPreviousLine);
              cCaret = mPreviousLineStartIndex + mCharPosition;
            }

            if (!event.key.shift) {
              cSelection = cCaret;
            }
            updateValues(style);
            break;
          }

          case sf::Keyboard::Down: {
            IFont* mFont = style.getCodeFont();
            float mFontSize = style.getCodeFontSize();

            std::string::size_type mLineEndIndex = cEditingCode.substr(cLineStartIndex).find_first_of('\n');
            if (mLineEndIndex == std::string::npos) {
              cCaret = cEditingCode.size();
            } else {
              mLineEndIndex += cLineStartIndex + 1;
              std::string::size_type mNextLineLength = cEditingCode.substr(mLineEndIndex).find_first_of('\n');
              std::string mNextLine = mNextLineLength == std::string::npos ? cEditingCode.substr(mLineEndIndex)
                                                                           : cEditingCode.substr(mLineEndIndex, mNextLineLength);
              unsigned int mCharPosition = mFont->getChar(cCaretXPosition, mFontSize, mNextLine);
              cCaret = mLineEndIndex + mCharPosition;
            }

            if (!event.key.shift) {
              cSelection = cCaret;
            }
            updateValues(style);
            break;
          }

          case sf::Keyboard::Home: {
            cCaret = cLineStartIndex;
            if (!event.key.shift) {
              cSelection = cCaret;
            }
            updateValues(style);
            updateCaretColumnPosition(style);

            if (!event.key.shift) {
              cSelection = cCaret;
            }
            break;
          }

          case sf::Keyboard::End: {
            unsigned int mLineEndIndex = cLineStartIndex;
            while (mLineEndIndex != cEditingCode.length() && cEditingCode[mLineEndIndex] != '\n') {
              mLineEndIndex++;
            }
            cCaret = mLineEndIndex;
            updateValues(style);
            updateCaretColumnPosition(style);

            if (!event.key.shift) {
              cSelection = cCaret;
            }
            break;
          }

          case sf::Keyboard::BackSpace: {
            if (cCaret != cSelection) {
              clearSelection(style);
            } else if (cCaret > 0) {

              cUndoStack.emplace(Edit(*this, false, cEditingCode.substr(cCaret - 1, 1), cCaret - 1));
              while (!cRedoStack.empty()) {cRedoStack.pop();}

              cEditingCode = cEditingCode.substr(0, cCaret - 1) + cEditingCode.substr(cCaret);
              cCaret--;
              cSelection = cCaret;
            }
            updateValues(style);
            updateCaretColumnPosition(style);
            break;
          }

          case sf::Keyboard::Return: {
            clearSelection(style);

            cUndoStack.emplace(Edit(*this, true, "\n", cCaret));
            while (!cRedoStack.empty()) {cRedoStack.pop();}

            cEditingCode = cEditingCode.substr(0, cCaret) + '\n' + cEditingCode.substr(cCaret);
            cCaret++;
            cSelection = cCaret;
            updateValues(style);
            updateCaretColumnPosition(style);
            break;
          }

          case sf::Keyboard::Tab: {
            clearSelection(style);

            cUndoStack.emplace(Edit(*this, true, "  ", cCaret));
            while (!cRedoStack.empty()) {cRedoStack.pop();}

            cEditingCode = cEditingCode.substr(0, cCaret) + "  " + cEditingCode.substr(cCaret);
            cCaret += 2;
            cSelection = cCaret;
            updateValues(style);
            updateCaretColumnPosition(style);
            break;
          }

          case sf::Keyboard::Escape: {
            if (cParent.cGetter() != cEditingCode) {
              cConfirmationSelection = std::make_unique<Choice>(style, "Would you like to keep your changes?", std::vector<std::string>{"Continue Editing", "Confirm Changes", "Discard Changes"}, [this](const std::string& choice)->bool {
                if (choice == "Confirm Changes") {
                  cParent.cSetter(cEditingCode);
                }
                if (choice != "Continue Editing") {
                  cClosing = true;
                }
                cClosedConfirmationSelection = std::move(cConfirmationSelection);
                cConfirmationSelection = nullptr;
                return cClosing;
              });
            } else {
              cClosing = true;
            }
            return cClosing;
          }

          case sf::Keyboard::X: {
            if (event.key.control) {
              cut(style);
            }
            break;
          }

          case sf::Keyboard::Delete: {
            if (event.key.shift) {
              cut(style);
            } else {
              if (cCaret != cSelection) {
                clearSelection(style);
              } else if (cCaret < cEditingCode.length()) {
                cEditingCode = cEditingCode.substr(0, cCaret) + cEditingCode.substr(cCaret + 1);
              }
            }
            break;
          }

          case sf::Keyboard::C: {
            if (event.key.control) {
              copy();
            }
            break;
          }

          case sf::Keyboard::Insert: {
            if (event.key.control) {
              copy();
            } else if (event.key.shift) {
              paste(style);
            }
            break;
          }

          case sf::Keyboard::V: {
            if (event.key.control) {
              paste(style);
            }
            break;
          }

          case sf::Keyboard::Z: {
            if (event.key.control) {
              if (event.key.shift) {
                redo(style);
              } else {
                undo(style);
              }
            }
            break;
          }

          case sf::Keyboard::Y: {
            if (event.key.control) {
              redo(style);
            }
            break;
          }

          default: {
            break;
          }
        }
        break;
      }
      default: break;
    }
    return false;
  }

  void PropertyCode::Editor::updateCaretColumnPosition(IUIStyle& style) {
    IFont* mFont = style.getCodeFont();
    float mFontSize = style.getCodeFontSize();
    cCaretXPosition = mFont->getWidth(mFontSize, cEditingCode.substr(cLineStartIndex, cCaret - cLineStartIndex));
  }

  void PropertyCode::Editor::updateValues(IUIStyle& style) {
    cLine = 0;
    cLineStartIndex = 0;
    for (unsigned int i = 0; i < cCaret; i++) {
      if (cEditingCode[i] == '\n') {
        cLine++;
        cLineStartIndex = i + 1;
      }
    }
    IFont* mFont = style.getCodeFont();
    float mFontSize = style.getCodeFontSize();
    cCaretOffsetX = mFont->getWidth(mFontSize, cEditingCode.substr(cLineStartIndex, cCaret - cLineStartIndex));
    
    // float mContentWidth      = mFont->getWidth( mFontSize, cEditingCode);
    // float mContentHeight     = mFont->getHeight(mFontSize, cEditingCode);
    float mAvailableWidth    = 2.0f - mFontSize * 2.0f;
    float mAvailableHeight   =               2.0f - mFontSize * 2.0f;
    // float mMaxScrollX        = mContentWidth  - mAvailableWidth;
    // float mMaxScrollY        = mContentHeight - mAvailableHeight;
    float mCurrentViewLeft   = cScrollX.value();
    float mCurrentViewRight  = cScrollX.value() + mAvailableWidth;
    float mCurrentViewTop    = cScrollY.value();
    float mCurrentViewBottom = cScrollY.value() + mAvailableHeight;
    if (mCurrentViewRight < cCaretOffsetX) {
      cScrollX = cCaretOffsetX - mAvailableWidth;
    } else if (mCurrentViewLeft > cCaretOffsetX) {
      cScrollX = cCaretOffsetX;
    }
    
    float mLineHeight = mFont->getHeight(mFontSize, "A");
    float mCaretTop = cLine * mLineHeight;
    float mCaretBottom = mCaretTop + mLineHeight * 2.5f; // TODO: This is wrong (* 2.5 should not be there), meaning mCurrentViewBottom is also probably wrong.
    if (mCurrentViewBottom < mCaretBottom) {
      cScrollY = mCaretBottom - mAvailableHeight;
    } else if (mCurrentViewTop > mCaretTop) {
      cScrollY = mCaretTop;
    }
  }

  void PropertyCode::Editor::clearSelection(IUIStyle& style) {
    if (cSelection != cCaret) {
      unsigned int mSelectionStart = std::min(cCaret, cSelection);
      unsigned int mSelectionEnd = std::max(cCaret, cSelection);

      cUndoStack.emplace(Edit(*this, false, cEditingCode.substr(mSelectionStart, mSelectionEnd - mSelectionStart), mSelectionStart));
      while (!cRedoStack.empty()) {cRedoStack.pop();}

      cEditingCode = cEditingCode.substr(0, mSelectionStart) + cEditingCode.substr(mSelectionEnd);
      cCaret = mSelectionStart;
      cSelection = cCaret;
      updateValues(style);
      updateCaretColumnPosition(style);
    }
  }

  void PropertyCode::Editor::cut(IUIStyle& style) {
    copy();
    clearSelection(style);
  }

  void PropertyCode::Editor::copy() {
    if (cCaret != cSelection) {
      unsigned int mSelectionStart = std::min(cCaret, cSelection);
      unsigned int mSelectionEnd = std::max(cCaret, cSelection);
      cClipboard = cEditingCode.substr(mSelectionStart, mSelectionEnd - mSelectionStart);
    }
  }

  void PropertyCode::Editor::paste(IUIStyle& style) {
    clearSelection(style);

    cUndoStack.emplace(Edit(*this, true, cClipboard, cCaret));
    while (!cRedoStack.empty()) {cRedoStack.pop();}

    cEditingCode = cEditingCode.substr(0, cCaret) + cClipboard + cEditingCode.substr(cCaret);
    cCaret += cClipboard.length();
    cSelection = cCaret;
    updateValues(style);
    updateCaretColumnPosition(style);
  }

  void PropertyCode::Editor::undo(IUIStyle& style) {
    if (!cUndoStack.empty()) {
      Edit& mEdit = cUndoStack.top();
      mEdit.undo(style);
      cRedoStack.emplace(mEdit);
      cUndoStack.pop();
    }
  }

  void PropertyCode::Editor::redo(IUIStyle& style) {
    if (!cRedoStack.empty()) {
      Edit& mEdit = cRedoStack.top();
      mEdit.redo(style);
      cUndoStack.emplace(mEdit);
      cRedoStack.pop();
    }
  }

  int PropertyCode::Editor::getCaretPosition(IUIStyle& style, int x, int y) {
    IApplication& mApplication = cParent.cProject.getApplication();
    Point2D mLocation = mApplication.normalise(x, y);
    IFont* mFont = style.getCodeFont();
    float mFontSize = style.getCodeFontSize();

    // Get X position in the code text.
    float mTotalCodeWidth = mFont->getWidth(mFontSize, cEditingCode);
    float mPanelWidth  = cOpenness / 250.0f * std::min(mTotalCodeWidth + mFontSize * 4.0f, (1.0f / mApplication.getScreenAspectRatio()) * 2.0f - mFontSize * 2.0f) ;
    float mTextLeft = -mPanelWidth / 2.0f + mFontSize * 2.0f;
    float mX = (mLocation.getX() - mTextLeft) + cScrollX.animation();

    // Get Y position in the code text.
    float mTotalCodeHeight = mFont->getHeight(mFontSize, cEditingCode);
    float mPanelHeight = cOpenness / 250.0f * std::min(mTotalCodeHeight + mFontSize * 4.0f, 2.0f - mFontSize * 2.0f);
    float mTextTop = (mPanelHeight / 2.0f - mFontSize * 2.0f);
    float mY = (-mLocation.getY() + mTextTop) + cScrollY.animation();

    // Translate Y position to line number in the text.
    float mLineHeight = mFont->getHeight(mFontSize, "A");
    int mLine = std::floor(mY / mLineHeight);

    // Now figure out exact caret position.
    float mSmallestDifference = std::numeric_limits<float>::max();
    int mCurrentLine = 0;
    int mLineStartIndex = 0;
    int mCaretPosition = 0;
    for (unsigned int i = 0; i <= cEditingCode.length(); i++) {
      if (mCurrentLine == mLine) {
        std::string mLine = cEditingCode.substr(mLineStartIndex, i - mLineStartIndex);
        float mWidth = mFont->getWidth(mFontSize, mLine);
        float mDifference = std::abs(mX - mWidth);
        if (mDifference < mSmallestDifference) {
          mSmallestDifference = mDifference;
          mCaretPosition = i;
        } else {
          break;
        }
      }
      if (cEditingCode[i] == '\n') {
        if (mLineStartIndex != 0) {
          break;
        }
        mCurrentLine++;
        if (mCurrentLine == mLine) {
          mLineStartIndex = i + 1;
        }
      }
    }
    return (mLine > mCurrentLine && mCaretPosition == 0) ? cEditingCode.length() : mCaretPosition;
  }

  PropertyCode::Editor::Edit::Edit(Editor& parent, bool addition, const std::string& content, unsigned int position) :
            cParent(parent),
            cAddition(addition),
            cContent(content),
            cPosition(position) {
  }

  void PropertyCode::Editor::Edit::undo(IUIStyle& style) {
    if (cAddition) {
      remove(style);
    } else {
      add(style);
    }
  }

  void PropertyCode::Editor::Edit::redo(IUIStyle& style) {
    if (cAddition) {
      add(style);
    } else {
      remove(style);
    }
  }

  void PropertyCode::Editor::Edit::remove(IUIStyle& style) {
    cParent.cEditingCode = cParent.cEditingCode.substr(0, cPosition) + cParent.cEditingCode.substr(cPosition + cContent.length());
    cParent.cCaret = cPosition;
    cParent.cSelection = cPosition;
    cParent.updateValues(style);
    cParent.updateCaretColumnPosition(style);
  }

  void PropertyCode::Editor::Edit::add(IUIStyle& style) {
    cParent.cEditingCode = cParent.cEditingCode.substr(0, cPosition) + cContent + cParent.cEditingCode.substr(cPosition);
    cParent.cCaret = cPosition + cContent.length();
    cParent.cSelection = cPosition + cContent.length();
    cParent.updateValues(style);
    cParent.updateCaretColumnPosition(style);
  }

  int PropertyCode::Editor::cDelayUntilBlinkChange = BLINK_DELAY;
  bool PropertyCode::Editor::cBlinkShowing = true;
}
