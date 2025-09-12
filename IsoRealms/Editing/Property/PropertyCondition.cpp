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
#include "PropertyCondition.h"

#include "IsoRealms/Common/ScreenArea.h"
#include "IsoRealms/Condition/Condition.h"
#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/Application.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Utils.h"

#include "IsoRealms/Editing/UISignalID.h"

#include "IPropertyManager.h"

namespace IsoRealms {
  PropertyCondition::PropertyCondition(const PropertyData& data, IResourceAccessManager& resourceAccessManager, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) :
            Property(data, resourceAccessManager, nullptr),
            cGetter(getter),
            cSetter(setter),
            cAvailableElements(availableElements) {
    cConditionString = getConditionString(cGetter());
  }

  void PropertyCondition::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    for (const std::unique_ptr<IConditionChar>& mChar : cConditionString) {
      mChar->render(style, x, y);
      x += mChar->getWidth(style);
    }
  }

  float PropertyCondition::getValueWidth(IUIStyle& style) const {
    float mWidth = 0.0f;
    for (const std::unique_ptr<IConditionChar>& mChar : cConditionString) {
      mWidth += mChar->getWidth(style);
    }
    return mWidth;
  }

  void PropertyCondition::confirm(IPropertyManager& manager, float y) {
    manager.edit(std::make_unique<Editor>(*this));
  }

  bool PropertyCondition::hasConfiguration() const {
    return false;
  }
  
  void PropertyCondition::configure(IPropertyManager& manager) {
    // Nothing to do.
  }
  
  /**********\
   * Editor *
  \**********/
  PropertyCondition::Editor::Editor(PropertyCondition& parent) :
            cParent(parent),
            cSelectedElement(0),
            cSelectionHeight(0.0f),
            cSelectionDepth(0.0f),
            cSelectedNotGate(false),
            cShowingConditionPalette(false),
            cOpenness(0),
            cClosing(false) {
    cConditionDiagram = std::make_unique<ResultOutput>(*this);
    cConditionDiagram->setInput(getConditionDiagram(*this, cParent.cGetter(), cConditionDiagram.get()));
    updatePosition(true);
  }
            
  void PropertyCondition::Editor::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_2D, 0);
    float mYScale = style.getFontSize();
    float mXScale = mYScale;
    float mXSpacing = 0.2f;
    float mYSpacing = mYScale * 2.5f;
    int mMaxDepth = cConditionDiagram->getMaxDepth();
    int mMaxHeight = cConditionDiagram->getElementCount() - 1;

    float mOpenLeft   = -mXScale * 2.0f - mMaxDepth * (mXSpacing + mXScale * 2.0f);
    float mOpenBottom = -mYScale * 2.0f;
    float mOpenRight  =  mXScale * 2.0f;
    float mOpenTop    =  mYScale * 2.0f + mMaxHeight * mYSpacing;

    mOpenLeft += (mXSpacing + mXScale * 2.0f) * cSelectionDepth.animation();
    mOpenRight += (mXSpacing + mXScale * 2.0f) * cSelectionDepth.animation();
    mOpenBottom += -cSelectionHeight.animation() * mYSpacing;
    mOpenTop += -cSelectionHeight.animation() * mYSpacing;

    float mClosedLeft   = x - mYScale * 2.0f;
    float mClosedRight  = x + cParent.getValueWidth(style) + mYScale * 2.0f;
    float mClosedBottom = y;
    float mClosedTop    = y + mYScale * 2.0f;

    float mFrameLeft   = mClosedLeft   + (mOpenLeft   - mClosedLeft)   * (cOpenness / 250.0f);
    float mFrameRight  = mClosedRight  + (mOpenRight  - mClosedRight)  * (cOpenness / 250.0f);
    float mFrameBottom = mClosedBottom + (mOpenBottom - mClosedBottom) * (cOpenness / 250.0f);
    float mFrameTop    = mClosedTop    + (mOpenTop    - mClosedTop)    * (cOpenness / 250.0f);

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
    glEnable(GL_BLEND);
    Utils::renderRoundedRectangle(mFrameLeft, mFrameBottom, mFrameRight, mFrameTop, mYScale * 2.0f );
    Application& mApplication = style.getProject().getApplication();
    ScreenArea mPreviousCrop = mApplication.crop(ScreenArea(mFrameLeft, mFrameRight, mFrameBottom, mFrameTop));
    glColor4f(1.0f, 0.0f, 0.2f, 1.0f);
    Utils::renderBar(0.0f, -mYScale * 1.2f, 0.0f, mYScale * 1.2f);
    glDepthMask(GL_TRUE);
    cConditionDiagram->render(style);
    mApplication.crop(mPreviousCrop);


    renderConditionPalette(style, cSelectedInput->getOutput()->isAnd());
  }

  bool PropertyCondition::Editor::update(unsigned int milliseconds) {
    cSelectionHeight.update(milliseconds);
    cSelectionDepth.update(milliseconds);
    cSelectedElement.update(milliseconds);
    if (cClosing) {
      cOpenness = std::max(cOpenness - static_cast<int>(milliseconds), 0);
    } else if (cOpenness < 250) {
      cOpenness = std::min(cOpenness + static_cast<int>(milliseconds), 250);
    }
    return cOpenness == 0;
  }

  bool PropertyCondition::Editor::input(UISignalID id, IUIStyle& style) {
    if (cShowingConditionPalette) {
      switch (id) {
        case UISignalID::MOVE_LEFT:  selectPreviousElement();          break;
        case UISignalID::MOVE_RIGHT: selectNextElement();              break;
        case UISignalID::CANCEL:     cShowingConditionPalette = false; break;
        case UISignalID::CONFIRM:    selectFromPalette();              break;
        default:                                                       break;
      }
    } else if (cSelectedInput->input(id)) {
      // Nothing to do.
    } else switch (id) {
      case UISignalID::MOVE_UP:     moveCursorUp();     break;
      case UISignalID::MOVE_DOWN:   moveCursorDown();   break;
      case UISignalID::MOVE_LEFT:   moveCursorLeft();   break;
      case UISignalID::MOVE_RIGHT:  moveCursorRight();  break;
      case UISignalID::CANCEL:      confirmCondition(); cClosing = true; break;
      default:                                          break;
    }
    return cClosing;
  }

  bool PropertyCondition::Editor::input(sf::Event& event, IUIStyle& style) {
    return false;
  }

  /*****************\
   * FunctionBlock *
  \*****************/
  PropertyCondition::Editor::FunctionBlock::FunctionBlock(Editor& parent, bool isAnd, bool isNot, IFunctionOutput* output) :
            cParent(parent),
            cOutput(output),
            cAnd(isAnd),
            cNot(isNot) {
  }

  void PropertyCondition::Editor::FunctionBlock::addInput(std::unique_ptr<IFunctionInput> input) {
    cInputs.push_back(std::move(input));
  }

  void PropertyCondition::Editor::FunctionBlock::confirm() {
    if (cParent.cSelectedNotGate) {
      cNot = !cNot;
    } else {
      cParent.showConditionPalette(cAnd);
    }
  }

  void PropertyCondition::Editor::FunctionBlock::render(IUIStyle& style, bool tick) const {
    float mYScale = style.getFontSize();
    float mXScale = mYScale;
    float mXSpacing = 0.2f;
    float mYSpacing = mYScale * 2.5f;

    float mHeightStep = mYSpacing * (getElementCount() - cInputs[0]->getElementCount());

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    Utils::renderRoundedRectangleLines(-mXScale, -mYScale, mXScale, mYScale + mHeightStep, mYScale);

    int mStep = 0;
    for (int i = static_cast<int>(cInputs.size()) - 1; i >= 0; i--) {
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      glBindTexture(GL_TEXTURE_2D, 0);
      glBegin(GL_LINES);
      glVertex3f(-mXScale,             mStep * mYSpacing, 0.0f);
      glVertex3f(-mXScale - mXSpacing, mStep * mYSpacing, 0.0f);
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(-mXScale,                           mStep * mYSpacing, 0.0f);
      glVertex3f(-mXScale * 1.5f, mYScale *  0.25f + mStep * mYSpacing, 0.0f);
      glVertex3f(-mXScale * 1.5f, mYScale * -0.25f + mStep * mYSpacing, 0.0f);
      glEnd();
      glPushMatrix();
      glTranslatef(-mXSpacing - mXScale * 2.0f, mStep * mYSpacing, 0.0f);
      cInputs[i]->render(style, cAnd);
      mStep += cInputs[i]->getElementCount();
      glPopMatrix();
    }
    IFont* mFont = style.getFont();
    mFont->print(0.0f, -mYScale * 0.8f + mHeightStep * 0.5f, mYScale, IFont::Alignment::CENTER, cAnd ? "&" : "|");

    if (cNot) {
      float mX = (mXSpacing + mXScale * 2.0f) * 0.5f;
      glBindTexture(GL_TEXTURE_2D, 0);
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      glBegin(GL_TRIANGLES);
      glVertex3f(mX - mXScale * 0.6f, +mYScale * 0.6f, 0.0f);
      glVertex3f(mX - mXScale * 0.6f, -mYScale * 0.6f, 0.0f);
      glVertex3f(mX + mXScale * 0.6f,            0.0f, 0.0f);
      glEnd();
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      glBegin(GL_LINE_LOOP);
      glVertex3f(mX - mXScale * 0.6f, +mYScale * 0.6f, 0.0f);
      glVertex3f(mX - mXScale * 0.6f, -mYScale * 0.6f, 0.0f);
      glVertex3f(mX + mXScale * 0.6f,            0.0f, 0.0f);
      glEnd();
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      Utils::renderCurve(mX + mXScale * 0.7f, 0.0f, mXScale * 0.2f, 0.0f, 1.0f);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      Utils::renderCurveLine(mX + mXScale * 0.7f, 0.0f, mXScale * 0.2f, 0.0f, 1.0f);
    }
  }

  int PropertyCondition::Editor::FunctionBlock::getElementCount() const {
    int mElementCount = 0;
    for (int i = 0; i < static_cast<int>(cInputs.size()); i++) {
      mElementCount += cInputs[i]->getElementCount();
    }
    return mElementCount;
  }

  bool PropertyCondition::Editor::FunctionBlock::input(UISignalID id) {
    switch (id) {
      case UISignalID::CONFIRM: confirm(); return true;
      default:                             break;
    }
    return false;
  }

  PropertyCondition::Editor::IFunctionOutput* PropertyCondition::Editor::FunctionBlock::getOutput() const {
    return cOutput;
  }

  PropertyCondition::Editor::IFunctionInput* PropertyCondition::Editor::FunctionBlock::getInput(int index) const {
    return cInputs[std::clamp(index, 0, static_cast<int>(cInputs.size() - 1))].get();
  }

  bool PropertyCondition::Editor::FunctionBlock::setAnd(bool isAnd) {
    cAnd = isAnd;
    return true;
  }

  std::optional<Condition> PropertyCondition::Editor::FunctionBlock::createCondition() {
    std::optional<Condition> mCondition = Condition(cAnd, cNot);
    for (std::unique_ptr<IFunctionInput>& mInput : cInputs) {
      std::optional<Condition> mSubCondition = mInput->createCondition();
      if (mSubCondition.has_value()) {
        mCondition->add(mSubCondition.value());
      }
    }
    return mCondition;
  }

  float PropertyCondition::Editor::FunctionBlock::getSelectedHeight() const {
    return (cParent.cSelectedNotGate ? 0.0f : (getElementCount() - cInputs[0]->getElementCount()) * 0.5f) + cOutput->getSelectedHeight(this);
  }

  int PropertyCondition::Editor::FunctionBlock::getMaxDepth() const {
    int mMaxDepth = 0;
    for (const std::unique_ptr<IFunctionInput>& mInput : cInputs) {
      mMaxDepth = std::max(mMaxDepth, mInput->getMaxDepth());
    }
    return mMaxDepth + 1;
  }

  int PropertyCondition::Editor::FunctionBlock::getIndex(IFunctionInput* input) const {
    for (int i = 0; i < static_cast<int>(cInputs.size()); i++) {
      if (input == cInputs[i].get()) {
        return i;
      }
    }
    throw ArgumentException("ERROR: PropertyCondition::FunctionBlock::getIndex: Specified function input not found.");
  }

  void PropertyCondition::Editor::FunctionBlock::setInput(int index, std::unique_ptr<IFunctionInput> input) {
    cInputs[index] = std::move(input);
    if (index == static_cast<int>(cInputs.size()) - 1) {
      addInput(std::make_unique<ElementInput>(cParent, nullptr, this));
    }
  }

  PropertyCondition::Editor::IFunctionInput* PropertyCondition::Editor::FunctionBlock::removeInput(int index) {
    if (index < static_cast<int>(cInputs.size() - 1)) {
      cInputs.erase(cInputs.begin() + index);
    }
    return cInputs[index].get();
  }

  PropertyCondition::Editor::IFunctionInput* PropertyCondition::Editor::FunctionBlock::getAsInput() {
    return this;
  }

  float PropertyCondition::Editor::FunctionBlock::getSelectedHeight(const IFunctionInput* input) const {
    int mStep = 0;
    for (int i = static_cast<int>(cInputs.size()) - 1; i >= 0; i--) {
      if (cInputs[i].get() == input) {
        return mStep + cOutput->getSelectedHeight(this);
      }
      mStep += cInputs[i]->getElementCount();
    }
    throw ArgumentException("ERROR: PropertyCondition::FunctionBlock::getSelectedHeight: Specified function input not found.");
  }

  bool PropertyCondition::Editor::FunctionBlock::isAnd() const {
    return cAnd;
  }

  /****************\
   * ElementInput *
  \****************/
  PropertyCondition::Editor::ElementInput::ElementInput(Editor& parent, ConditionElement::Clause* input, IFunctionOutput* output) :
            cParent(parent),
            cInput(input),
            cOutput(output) {
  }

  void PropertyCondition::Editor::ElementInput::confirm() {
    if (cParent.cSelectedNotGate) {
      if (cInput != nullptr) {
        cInput = cInput->getNegatedClause();
      }
    } else {
      cParent.showConditionPalette(cInput != nullptr ? cInput->getElement() : nullptr);
    }
  }

  void PropertyCondition::Editor::ElementInput::render(IUIStyle& style, bool tick) const {
    float mYScale = style.getFontSize();
    float mXScale = mYScale;
    float mXSpacing = 0.2f;

    if (this != cParent.cSelectedInput || !cParent.cShowingConditionPalette) {
      glPushMatrix();
      if (cInput == nullptr) {
        glScalef(mXScale, mYScale, 1.0f);
        glDisable(GL_DEPTH_TEST);
        if (tick) {
          Utils::renderIconTick();
        } else {
          Utils::renderIconNone();
        }
      } else {
        glScalef(mYScale * 2.0f, mYScale * 2.0f, 1.0f);
        cInput->getElement()->renderIcon();
        glDisable(GL_DEPTH_TEST);
      }
      glPopMatrix();
    }

    if (cInput != nullptr && cInput->isNegated()) {
      float mX = (mXSpacing + mXScale * 2.0f) * 0.5f;
      glBindTexture(GL_TEXTURE_2D, 0);
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      glBegin(GL_TRIANGLES);
      glVertex3f(mX - mXScale * 0.6f, +mYScale * 0.6f, 0.0f);
      glVertex3f(mX - mXScale * 0.6f, -mYScale * 0.6f, 0.0f);
      glVertex3f(mX + mXScale * 0.6f,            0.0f, 0.0f);
      glEnd();
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      glBegin(GL_LINE_LOOP);
      glVertex3f(mX - mXScale * 0.6f, +mYScale * 0.6f, 0.0f);
      glVertex3f(mX - mXScale * 0.6f, -mYScale * 0.6f, 0.0f);
      glVertex3f(mX + mXScale * 0.6f,            0.0f, 0.0f);
      glEnd();
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      Utils::renderCurve(mX + mXScale * 0.7f, 0.0f, mXScale * 0.2f, 0.0f, 1.0f);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      Utils::renderCurveLine(mX + mXScale * 0.7f, 0.0f, mXScale * 0.2f, 0.0f, 1.0f);
    }
  }

  int PropertyCondition::Editor::ElementInput::getElementCount() const {
    return 1;
  }

  bool PropertyCondition::Editor::ElementInput::input(UISignalID id) {
    switch (id) {
      case UISignalID::CONFIRM: confirm(); return true;
      default:                             break;
    }
    return false;
  }

  PropertyCondition::Editor::IFunctionOutput* PropertyCondition::Editor::ElementInput::getOutput() const {
    return cOutput;
  }

  PropertyCondition::Editor::IFunctionInput* PropertyCondition::Editor::ElementInput::getInput(int index) const {
    return nullptr;
  }

  bool PropertyCondition::Editor::ElementInput::setAnd(bool isAnd) {
    return false;
  }

  std::optional<Condition> PropertyCondition::Editor::ElementInput::createCondition() {
    if (cInput != nullptr) {
      std::optional<Condition> mCondition = Condition(false, false);
      mCondition->add(cInput);
      return mCondition;
    }
    return std::nullopt;
  }

  float PropertyCondition::Editor::ElementInput::getSelectedHeight() const {
    return cOutput != nullptr ? cOutput->getSelectedHeight(this) : 0.0f;
  }

  int PropertyCondition::Editor::ElementInput::getMaxDepth() const {
    return 1;
  }

  /****************\
   * ResultOutput *
  \****************/
  PropertyCondition::Editor::ResultOutput::ResultOutput(Editor& parent) :
            cParent(parent) {
  }

  void PropertyCondition::Editor::ResultOutput::setInput(std::unique_ptr<IFunctionInput> input) {
    cInput = std::move(input);
  }

  void PropertyCondition::Editor::ResultOutput::render(IUIStyle& style) {
    glPushMatrix();
    float mYScale = style.getFontSize();
    float mXScale = mYScale;
    float mXSpacing = 0.2f;
    float mYSpacing = mYScale * 2.5f;

    glTranslatef((mXSpacing + mXScale * 2.0f) * cParent.cSelectionDepth.animation(), -cParent.cSelectionHeight.animation() * mYSpacing, 0.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-mXScale,             0.0f, 0.0f);
    glVertex3f(-mXScale - mXSpacing, 0.0f, 0.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex3f(-mXScale,                   0.0f,  0.0f);
    glVertex3f(-mXScale * 1.5f, mYScale *  0.25f, 0.0f);
    glVertex3f(-mXScale * 1.5f, mYScale * -0.25f, 0.0f);
    glEnd();

    glPushMatrix();
    glDepthMask(GL_FALSE);
    glScalef(mXScale, mYScale, 1.0f);
    Utils::renderIconCustom();
    glDepthMask(GL_TRUE);
    glPopMatrix();

    glTranslatef(-mXSpacing - mXScale * 2.0f, 0.0f, 0.0f);
    cInput->render(style, true);
    glPopMatrix();
  }

  int PropertyCondition::Editor::ResultOutput::getMaxDepth() const {
    return cInput->getMaxDepth();
  }

  int PropertyCondition::Editor::ResultOutput::getElementCount() const {
    return cInput->getElementCount();
  }

  std::optional<Condition> PropertyCondition::Editor::ResultOutput::createCondition() {
    return cInput->createCondition();
  }

  int PropertyCondition::Editor::ResultOutput::getIndex(IFunctionInput* input) const {
    return 0;
  }

  PropertyCondition::Editor::IFunctionInput* PropertyCondition::Editor::ResultOutput::getInput(int index) const {
    return cInput.get();
  }

  void PropertyCondition::Editor::ResultOutput::setInput(int index, std::unique_ptr<IFunctionInput> input) {
    cInput = std::move(input);
  }

  PropertyCondition::Editor::IFunctionInput* PropertyCondition::Editor::ResultOutput::removeInput(int index) {
    cInput = std::make_unique<ElementInput>(cParent, nullptr, this);
    return cInput.get();
  }

  PropertyCondition::Editor::IFunctionInput* PropertyCondition::Editor::ResultOutput::getAsInput() {
    return nullptr;
  }

  float PropertyCondition::Editor::ResultOutput::getSelectedHeight(const IFunctionInput* input) const {
    return 0.0f;
  }

  bool PropertyCondition::Editor::ResultOutput::isAnd() const {
    return true;
  }
  
  /******************************\
   * Editor (Private Functions) *
  \******************************/
  void PropertyCondition::Editor::showConditionPalette(bool isAnd) {
    cShowingConditionPalette = true;
    cSelectedElement.init(static_cast<float>(isAnd ? 1 : 2));
  }

  void PropertyCondition::Editor::showConditionPalette(ConditionElement* element) {
    cShowingConditionPalette = true;
    if (element == nullptr) {
      cSelectedElement.init(0);
    } else {
      for (unsigned int i = 0; i < cParent.cAvailableElements.size(); i++) {
        if (element == cParent.cAvailableElements[i]) {
          cSelectedElement.init(static_cast<float>(i + 3));
          break;
        }
      }
    }
  }

  void PropertyCondition::Editor::renderConditionPalette(IUIStyle& style, bool tick) const {
    if (cShowingConditionPalette) {
      glDepthMask(GL_FALSE);
      glDisable(GL_DEPTH_TEST);
      glBindTexture(GL_TEXTURE_2D, 0);
      float mYScale = style.getFontSize();
      float mXScale = mYScale;
      float mSpacing = 0.02f;

      glPushMatrix();
      glTranslatef(-(mXScale * 2.0f + mSpacing) * cSelectedElement.animation(), 0.0f, 0.0f);
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      Utils::renderBar(0.0f, -mYScale * 1.2f, (mXScale * 2.0f + mSpacing) * (cParent.cAvailableElements.size() + 2), mYScale * 1.2f);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      Utils::renderRoundedRectangleLines(-mXScale * 1.2f, -mYScale * 1.2f, mXScale * 1.2f + (mXScale * 2.0f + mSpacing) * (cParent.cAvailableElements.size() + 2), mYScale * 1.2f, mYScale * 1.2f);
      glPopMatrix();

      glColor4f(1.0f, 0.0f, 0.2f, 1.0f);
      Utils::renderBar(0.0f, -mYScale * 1.2f, 0.0f, mYScale * 1.2f);

      glPushMatrix();
      glTranslatef(-(mXScale * 2.0f + mSpacing) * cSelectedElement.animation(), 0.0f, 0.0f);

      glPushMatrix();
      glScalef(mXScale, mYScale, 1.0f);
      glDisable(GL_DEPTH_TEST);
      if (tick) {
        Utils::renderIconTick();
      } else {
        Utils::renderIconNone();
      }
      glPopMatrix();
      IFont* mFont = style.getFont();

      glTranslatef(mXScale * 2.0f + mSpacing, 0.0f, 0.0f);
      mFont->print(0.0f, -mYScale * 0.8f, mYScale, IFont::Alignment::CENTER, "&");
      glTranslatef(mXScale * 2.0f + mSpacing, 0.0f, 0.0f);
      mFont->print(0.0f, -mYScale * 0.8f, mYScale, IFont::Alignment::CENTER, "|");

      for (int i = 0; i < static_cast<int>(cParent.cAvailableElements.size()); i++) {
        glTranslatef(mXScale * 2.0f + mSpacing, 0.0f, 0.0f);
        glPushMatrix();
        glScalef(mYScale * 2.0f, mYScale * 2.0f, 1.0f);
        cParent.cAvailableElements[i]->renderIcon();
        glDisable(GL_DEPTH_TEST);
        glPopMatrix();
      }
      glDepthMask(GL_TRUE);
      glPopMatrix();
    }
  }

  void PropertyCondition::Editor::updatePosition(bool init) {
    float mSelectionHeight = cSelectedInput->getSelectedHeight();
    int mSelectionDepthCount = 0;
    IFunctionInput* mInput = cSelectedInput;
    while (mInput != nullptr) {
      IFunctionOutput* mOutput =  mInput->getOutput();
      mInput = mOutput->getAsInput();
      mSelectionDepthCount++;
    }
    float mSelectionDepth = mSelectionDepthCount - (cSelectedNotGate ? 0.5f : 0.0f);
    if (init) {
      cSelectionHeight.init(mSelectionHeight);
      cSelectionDepth.init(mSelectionDepth);
    } else {
      cSelectionHeight = mSelectionHeight;
      cSelectionDepth = mSelectionDepth;
    }
  }

  void PropertyCondition::Editor::selectFromPalette() {
    IFunctionOutput* mOutput = cSelectedInput->getOutput();
    int mIndex = mOutput->getIndex(cSelectedInput);
    switch (static_cast<int>(cSelectedElement.value())) {
      case 0: {
        cSelectedInput = mOutput->removeInput(mIndex);
        updatePosition();
        break;
      }

      case 1:
      case 2: {
        if (!cSelectedInput->setAnd(static_cast<int>(cSelectedElement.value()) == 1)) {
          std::unique_ptr<FunctionBlock> mFunctionBlock = std::make_unique<FunctionBlock>(*this, cSelectedElement.value() == 1, false, mOutput);
          mFunctionBlock->addInput(std::make_unique<ElementInput>(*this, nullptr, mFunctionBlock.get()));
          cSelectedInput = mFunctionBlock.get();
          mOutput->setInput(mIndex, std::move(mFunctionBlock));
          updatePosition();
        }
        break;
      }

      default: {
        std::unique_ptr<ElementInput> mElementInput = std::make_unique<ElementInput>(*this, cParent.cAvailableElements[static_cast<int>(cSelectedElement.value()) - 3]->getPositiveClause(), mOutput);
        cSelectedInput = mElementInput.get();
        mOutput->setInput(mIndex, std::move(mElementInput));
        updatePosition();
        break;
      }
    }
    cShowingConditionPalette = false;
  }

  void PropertyCondition::Editor::selectNextElement() {
    cSelectedElement = static_cast<float>(std::min(static_cast<int>(cParent.cAvailableElements.size() + 2), static_cast<int>(cSelectedElement.value()) + 1));
  }

  void PropertyCondition::Editor::selectPreviousElement() {
    cSelectedElement = static_cast<float>(std::max(0, static_cast<int>(cSelectedElement.value()) - 1));
  }

  void PropertyCondition::Editor::confirmCondition() {
    std::optional<Condition> mCondition = cConditionDiagram->createCondition();
    if (mCondition.has_value()) {
      mCondition->simplify();
    }
    cParent.cSetter(mCondition);
    cParent.cConditionString = getConditionString(mCondition);
    cConditionDiagram = std::make_unique<ResultOutput>(*this);
    cConditionDiagram->setInput(getConditionDiagram(*this, mCondition, cConditionDiagram.get()));
    updatePosition(true);
  }

  void PropertyCondition::Editor::moveCursorDown() {
    IFunctionOutput* mOutput = cSelectedInput->getOutput();
    int mIndex = mOutput->getIndex(cSelectedInput);
    cSelectedInput = mOutput->getInput(mIndex + 1);
    updatePosition();
  }

  void PropertyCondition::Editor::moveCursorUp() {
    IFunctionOutput* mOutput = cSelectedInput->getOutput();
    int mIndex = mOutput->getIndex(cSelectedInput);
    cSelectedInput = mOutput->getInput(mIndex - 1);
    updatePosition();
  }

  void PropertyCondition::Editor::moveCursorLeft() {
    if (cSelectedNotGate) {
      cSelectedNotGate = false;
      updatePosition();
    } else {
      IFunctionInput* mInput = cSelectedInput->getInput(0);
      if (mInput != nullptr) {
        cSelectedInput = mInput;
        cSelectedNotGate = true;
        updatePosition();
      }
    }
  }

  void PropertyCondition::Editor::moveCursorRight() {
    if (cSelectedNotGate) {
      IFunctionOutput* mOutput = cSelectedInput->getOutput();
      IFunctionInput* mInput = mOutput->getAsInput();
      if (mInput != nullptr) {
        cSelectedInput = mInput;
        cSelectedNotGate = false;
        updatePosition();
      }
    } else {
      cSelectedNotGate = true;
      updatePosition();
    }
  }

  /************\
   * TextChar *
  \************/
  PropertyCondition::TextChar::TextChar(char character) :
            cCharacter(character) {
  }

  void PropertyCondition::TextChar::render(IUIStyle& style, float x, float y) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x, y, mFontSize, IFont::Alignment::LEFT, std::string(1, cCharacter));
  }

  float PropertyCondition::TextChar::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, std::string(1, cCharacter));
  }

  /***************\
   * ElementChar *
  \***************/
  PropertyCondition::ElementChar::ElementChar(ConditionElement::Clause* character) :
            cCharacter(character) {
  }

  void PropertyCondition::ElementChar::render(IUIStyle& style, float x, float y) const {
    float mFontSize = style.getFontSize();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
    glDisable(GL_DEPTH_TEST);
    if (cCharacter != nullptr) {
      glScalef(mFontSize * 2.0f, mFontSize * 2.0f, 1.0f);
      cCharacter->getElement()->renderIcon();
    } else {
      glScalef(mFontSize, mFontSize, 1.0f);
      Utils::renderIconTick();
    }
    glPopMatrix();
  }

  float PropertyCondition::ElementChar::getWidth(IUIStyle& style) const {
    return style.getFontSize() * 2.0f;
  }
  
  /********************\
   * Static Functions *
  \********************/
  std::vector<std::unique_ptr<PropertyCondition::IConditionChar>> PropertyCondition::getConditionString(std::optional<Condition>& condition) {
    std::vector<std::unique_ptr<IConditionChar>> mConditionString;
    if (condition.has_value()) {
      std::set<ConditionElement::Clause*> mConditionElements = condition->getConditionElements();
      std::vector<Condition> mSubConditions = condition->getConditions();
      char mOperatorChar = condition->isAnd() ? '&' : '|';
      unsigned int mPartsToGo = static_cast<unsigned int>(mConditionElements.size() + mSubConditions.size());
      if (condition->isNegated()) {
        mConditionString.emplace_back(std::make_unique<TextChar>('!'));
      }
      mConditionString.emplace_back(std::make_unique<TextChar>('('));
      for (Condition& mSubCondition : mSubConditions) {
        std::optional<Condition> mOptionalSubCondition(mSubCondition);
        std::vector<std::unique_ptr<PropertyCondition::IConditionChar>> mSubConditionString = getConditionString(mOptionalSubCondition);
        for (std::unique_ptr<IConditionChar>& mSubConditionChar : mSubConditionString) {
          mConditionString.emplace_back(std::move(mSubConditionChar));
        }
        if (--mPartsToGo > 0) {
          mConditionString.emplace_back(std::make_unique<TextChar>(mOperatorChar));
        }
      }
      for (ConditionElement::Clause* mElement : mConditionElements) {
        if (mElement->isNegated()) {
          mConditionString.emplace_back(std::make_unique<TextChar>('!'));
        }
        mConditionString.emplace_back(std::make_unique<ElementChar>(mElement));
        if (--mPartsToGo > 0) {
          mConditionString.emplace_back(std::make_unique<TextChar>(mOperatorChar));
        }
      }
      mConditionString.emplace_back(std::make_unique<TextChar>(')'));
    } else {
      mConditionString.emplace_back(std::make_unique<ElementChar>(nullptr));
    }
    return mConditionString;
  }

  std::unique_ptr<PropertyCondition::Editor::IFunctionInput> PropertyCondition::Editor::getConditionDiagram(PropertyCondition::Editor& editor, std::optional<Condition>& condition, IFunctionOutput* output) {
    std::vector<IConditionChar*> mConditionString;
    if (condition.has_value()) {
      std::vector<Condition> mSubConditions = condition->getConditions();
      std::set<ConditionElement::Clause*> mConditionElements = condition->getConditionElements();
      if (mSubConditions.empty() && mConditionElements.size() == 1) {
        for (ConditionElement::Clause* mElement : mConditionElements) {
          std::unique_ptr<ElementInput> mSingleElementInput = std::make_unique<ElementInput>(editor, mElement, output);
          editor.cSelectedInput = mSingleElementInput.get();
          return mSingleElementInput;
        }
      } else {
        std::unique_ptr<FunctionBlock> mFunctionBlock = std::make_unique<FunctionBlock>(editor, condition->isAnd(), condition->isNegated(), output);

        for (Condition& mSubCondition : mSubConditions) {
          std::optional<Condition> mOptionalSubCondition(mSubCondition);
          mFunctionBlock->addInput(getConditionDiagram(editor, mOptionalSubCondition, mFunctionBlock.get()));
        }

        for (ConditionElement::Clause* mElement : mConditionElements) {
          mFunctionBlock->addInput(std::make_unique<ElementInput>(editor, mElement, mFunctionBlock.get()));
        }
        std::unique_ptr<ElementInput> mElementInput = std::make_unique<ElementInput>(editor, nullptr, mFunctionBlock.get());
        editor.cSelectedInput = mElementInput.get();
        mFunctionBlock->addInput(std::move(mElementInput));
        return mFunctionBlock;
      }
    }
    std::unique_ptr<ElementInput> mUnconditionalInput = std::make_unique<ElementInput>(editor, nullptr, output);
    editor.cSelectedInput = mUnconditionalInput.get();
    return mUnconditionalInput;
  }
}
