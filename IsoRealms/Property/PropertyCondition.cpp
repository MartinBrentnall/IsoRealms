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
#include "PropertyCondition.h"

namespace IsoRealms {
  PropertyCondition::PropertyCondition(HatHandler& hatHandler, const std::string& label, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) :
            Property(label),
            cHatHandler(hatHandler),
            cGetter(getter),
            cSetter(setter),
            cAvailableElements(availableElements),
            cSelectedElement(0),
            cSelectionHeight(0.0f),
            cSelectionDepth(0.0f),
            cSelectedNotGate(false),
            cShowingConditionPalette(false) {
    cConditionString = getConditionString(this, cGetter());
    cConditionDiagram = std::make_unique<ResultOutput>(this);
    cConditionDiagram->setInput(getConditionDiagram(this, cGetter(), cConditionDiagram.get()));
    updatePosition(true);
  }

  std::vector<std::unique_ptr<PropertyCondition::IConditionChar>> PropertyCondition::getConditionString(PropertyCondition* parent, std::optional<Condition>& condition) {
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
        std::vector<std::unique_ptr<PropertyCondition::IConditionChar>> mSubConditionString = getConditionString(parent, mOptionalSubCondition);
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

  bool PropertyCondition::updateEditing(unsigned int milliseconds) {
    cSelectionHeight.update(milliseconds);
    cSelectionDepth.update(milliseconds);
    cSelectedElement.update(milliseconds);
    return true;
  }

  void PropertyCondition::render(IPropertyAppearance* appearance) const {
    float mXOffset = 0.0f;
    for (const std::unique_ptr<IConditionChar>& mChar : cConditionString) {
      mChar->render(appearance, mXOffset);
      mXOffset += mChar->getWidth(appearance);
    }
  }

  std::unique_ptr<PropertyCondition::IFunctionInput> PropertyCondition::getConditionDiagram(PropertyCondition* parent, std::optional<Condition>& condition, IFunctionOutput* output) {
    std::vector<IConditionChar*> mConditionString;
    if (condition.has_value()) {
      std::vector<Condition> mSubConditions = condition->getConditions();
      std::set<ConditionElement::Clause*> mConditionElements = condition->getConditionElements();
      if (mSubConditions.empty() && mConditionElements.size() == 1) {
        for (ConditionElement::Clause* mElement : mConditionElements) {
          std::unique_ptr<ElementInput> mSingleElementInput = std::make_unique<ElementInput>(parent, mElement, output);
          parent->cSelectedInput = mSingleElementInput.get();
          return mSingleElementInput;
        }
      } else {
        std::unique_ptr<FunctionBlock> mFunctionBlock = std::make_unique<FunctionBlock>(parent, condition->isAnd(), condition->isNegated(), output);

        for (Condition& mSubCondition : mSubConditions) {
          std::optional<Condition> mOptionalSubCondition(mSubCondition);
          mFunctionBlock->addInput(getConditionDiagram(parent, mOptionalSubCondition, mFunctionBlock.get()));
        }

        for (ConditionElement::Clause* mElement : mConditionElements) {
          mFunctionBlock->addInput(std::make_unique<ElementInput>(parent, mElement, mFunctionBlock.get()));
        }
        std::unique_ptr<ElementInput> mElementInput = std::make_unique<ElementInput>(parent, nullptr, mFunctionBlock.get());
        parent->cSelectedInput = mElementInput.get();
        mFunctionBlock->addInput(std::move(mElementInput));
        return mFunctionBlock;
      }
    }
    std::unique_ptr<ElementInput> mUnconditionalInput = std::make_unique<ElementInput>(parent, nullptr, output);
    parent->cSelectedInput = mUnconditionalInput.get();
    return mUnconditionalInput;
  }

  void PropertyCondition::showConditionPalette(bool isAnd) {
    cShowingConditionPalette = true;
    cSelectedElement.init(static_cast<float>(isAnd ? 1 : 2));
  }

  void PropertyCondition::showConditionPalette(ConditionElement* element) {
    cShowingConditionPalette = true;
    if (element == nullptr) {
      cSelectedElement.init(0);
    } else {
      for (unsigned int i = 0; i < cAvailableElements.size(); i++) {
        if (element == cAvailableElements[i]) {
          cSelectedElement.init(static_cast<float>(i + 3));
          break;
        }
      }
    }
  }

  void PropertyCondition::renderConditionPalette(IPropertyAppearance* appearance, bool tick) const {
    if (cShowingConditionPalette) {
      glDepthMask(GL_FALSE);
      glDisable(GL_DEPTH_TEST);
      glBindTexture(GL_TEXTURE_2D, 0);
      float mYScale = 0.05f;
      float mXScale = mYScale;
      float mSpacing = 0.02f;

      glPushMatrix();
      glTranslatef(-(mXScale * 2.0f + mSpacing) * cSelectedElement.animation(), 0.0f, 0.0f);
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      Utils::renderBar(0.0f, -mYScale * 1.2f, (mXScale * 2.0f + mSpacing) * (cAvailableElements.size() + 2), mYScale * 1.2f);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      Utils::renderRoundedRectangleLines(-mXScale * 1.2f, -mYScale * 1.2f, mXScale * 1.2f + (mXScale * 2.0f + mSpacing) * (cAvailableElements.size() + 2), mYScale * 1.2f, mYScale * 1.2f);
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

      glTranslatef(mXScale * 2.0f + mSpacing, 0.0f, 0.0f);
      appearance->print(0.0f, -mYScale * 0.8f, mYScale, IFont::Alignment::CENTER, "&");
      glTranslatef(mXScale * 2.0f + mSpacing, 0.0f, 0.0f);
      appearance->print(0.0f, -mYScale * 0.8f, mYScale, IFont::Alignment::CENTER, "|");

      for (int i = 0; i < static_cast<int>(cAvailableElements.size()); i++) {
        glTranslatef(mXScale * 2.0f + mSpacing, 0.0f, 0.0f);
        glPushMatrix();
        glScalef(mYScale * 2.0f, mYScale * 2.0f, 1.0f);
        cAvailableElements[i]->renderIcon();
        glDisable(GL_DEPTH_TEST);
        glPopMatrix();
      }
      glDepthMask(GL_TRUE);
      glPopMatrix();
    }
  }

  void PropertyCondition::updatePosition(bool init) {
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

  void PropertyCondition::renderEditing(IPropertyAppearance* appearance) const {
    glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_2D, 0);
    float mYScale = 0.05f;
    float mXScale = mYScale;
    float mXSpacing = 0.2f;
    float mYSpacing = mYScale * 2.5f;
    int mMaxDepth = cConditionDiagram->getMaxDepth();
    int mMaxHeight = cConditionDiagram->getElementCount() - 1;

    glPushMatrix();
    glTranslatef((mXSpacing + mXScale * 2.0f) * cSelectionDepth.animation(), -cSelectionHeight.animation() * mYSpacing, 0.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
    glEnable(GL_BLEND);
    Utils::renderRoundedRectangle(-mXScale * 2.0f - mMaxDepth * (mXSpacing + mXScale * 2.0f), -mYScale * 2.0f , mXScale * 2.0f , mYScale * 2.0f  + mMaxHeight * mYSpacing, mYScale * 2.0f );
    glPopMatrix();
    glColor4f(1.0f, 0.0f, 0.2f, 1.0f);
    Utils::renderBar(0.0f, -mYScale * 1.2f, 0.0f, mYScale * 1.2f);
    glDepthMask(GL_TRUE);
    cConditionDiagram->render(appearance);


    renderConditionPalette(appearance, cSelectedInput->getOutput()->isAnd());
  }

  IEditable* PropertyCondition::edit(IAssetRegistry* assets) {
    return nullptr;
  }

  void PropertyCondition::close() {
  }

  float PropertyCondition::getWidth(IPropertyAppearance* appearance) const {
    float mWidth = 0.0f;
    for (const std::unique_ptr<IConditionChar>& mChar : cConditionString) {
      mWidth += mChar->getWidth(appearance);
    }
    return mWidth;
  }

  void PropertyCondition::selectFromPalette() {
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
          std::unique_ptr<FunctionBlock> mFunctionBlock = std::make_unique<FunctionBlock>(this, cSelectedElement.value() == 1, false, mOutput);
          mFunctionBlock->addInput(std::make_unique<ElementInput>(this, nullptr, mFunctionBlock.get()));
          cSelectedInput = mFunctionBlock.get();
          mOutput->setInput(mIndex, std::move(mFunctionBlock));
          updatePosition();
        }
        break;
      }

      default: {
        std::unique_ptr<ElementInput> mElementInput = std::make_unique<ElementInput>(this, cAvailableElements[static_cast<int>(cSelectedElement.value()) - 3]->getPositiveClause(), mOutput);
        cSelectedInput = mElementInput.get();
        mOutput->setInput(mIndex, std::move(mElementInput));
        updatePosition();
        break;
      }
    }
    cShowingConditionPalette = false;
  }

  void PropertyCondition::selectNextElement() {
    cSelectedElement = static_cast<float>(std::min(static_cast<int>(cAvailableElements.size() + 2), static_cast<int>(cSelectedElement.value()) + 1));
  }

  void PropertyCondition::selectPreviousElement() {
    cSelectedElement = static_cast<float>(std::max(0, static_cast<int>(cSelectedElement.value()) - 1));
  }

  void PropertyCondition::confirmCondition() {
    std::optional<Condition> mCondition = cConditionDiagram->createCondition();
    if (mCondition.has_value()) {
      mCondition->simplify();
    }
    cSetter(mCondition);
    cConditionString = getConditionString(this, mCondition);
    cConditionDiagram = std::make_unique<ResultOutput>(this);
    cConditionDiagram->setInput(getConditionDiagram(this, mCondition, cConditionDiagram.get()));
    updatePosition(true);
  }

  void PropertyCondition::moveCursorDown() {
    IFunctionOutput* mOutput = cSelectedInput->getOutput();
    int mIndex = mOutput->getIndex(cSelectedInput);
    cSelectedInput = mOutput->getInput(mIndex + 1);
    updatePosition();
  }

  void PropertyCondition::moveCursorUp() {
    IFunctionOutput* mOutput = cSelectedInput->getOutput();
    int mIndex = mOutput->getIndex(cSelectedInput);
    cSelectedInput = mOutput->getInput(mIndex - 1);
    updatePosition();
  }

  void PropertyCondition::moveCursorLeft() {
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

  void PropertyCondition::moveCursorRight() {
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

  bool PropertyCondition::input(ConfiguratorSignalID id) {
    if (cShowingConditionPalette) {
      switch (id) {
        case ConfiguratorSignalID::MOVE_LEFT:  selectPreviousElement();          break;
        case ConfiguratorSignalID::MOVE_RIGHT: selectNextElement();              break;
        case ConfiguratorSignalID::CANCEL:     cShowingConditionPalette = false; break;
        case ConfiguratorSignalID::CONFIRM:    selectFromPalette();              break;
        default:                                                                 break;
      }
    } else if (cSelectedInput->input(id)) {
      return false;
    } else switch (id) {
      case ConfiguratorSignalID::MOVE_UP:     moveCursorUp();     break;
      case ConfiguratorSignalID::MOVE_DOWN:   moveCursorDown();   break;
      case ConfiguratorSignalID::MOVE_LEFT:   moveCursorLeft();   break;
      case ConfiguratorSignalID::MOVE_RIGHT:  moveCursorRight();  break;
      case ConfiguratorSignalID::CANCEL:      confirmCondition(); return true;
      default:                                                    break;
    }
    return false;
  }

  PropertyCondition::TextChar::TextChar(char character) :
            cCharacter(character) {
  }

  void PropertyCondition::TextChar::render(IPropertyAppearance* appearance, float xOffset) const {
    appearance->print(std::string(1, cCharacter), 0, xOffset);
  }

  float PropertyCondition::TextChar::getWidth(IPropertyAppearance* appearance) const {
    return appearance->getWidth(std::string(1, cCharacter));
  }

  PropertyCondition::ElementChar::ElementChar(ConditionElement::Clause* character) :
            cCharacter(character) {
  }

  void PropertyCondition::ElementChar::render(IPropertyAppearance* appearance, float xOffset) const {
    glBindTexture(GL_TEXTURE_2D, 0);
    float mY      = appearance->getY(0);
    float mScale  = appearance->getIconWidth();
//     float mLeft   = -mScale;
//     float mRight  =  mScale;
//     float mTop    =  mScale;
//     float mBottom = -mScale;
    glPushMatrix();
// TODO   ScreenArea mPreviousCrop = mApplication->crop(ScreenArea(mLeft, mRight, mBottom, mTop));
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(xOffset + mScale * 0.5f, mY, 0.0f);
    glScalef(mScale, mScale, 1.0f);
    glDisable(GL_DEPTH_TEST);
    if (cCharacter != nullptr) {
      cCharacter->getElement()->renderIcon();
    } else {
      Utils::renderIconTick();
    }
//    mApplication->crop(mPreviousCrop);
    glPopMatrix();
  }

  float PropertyCondition::ElementChar::getWidth(IPropertyAppearance* appearance) const {
    return appearance->getIconWidth();
  }

  PropertyCondition::FunctionBlock::FunctionBlock(PropertyCondition* parent, bool isAnd, bool isNot, IFunctionOutput* output) :
            cParent(parent),
            cOutput(output),
            cAnd(isAnd),
            cNot(isNot) {
  }

  void PropertyCondition::FunctionBlock::addInput(std::unique_ptr<IFunctionInput> input) {
    cInputs.push_back(std::move(input));
  }

  void PropertyCondition::FunctionBlock::render(IPropertyAppearance* appearance, bool tick) const {
    float mYScale = 0.05f;
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
      cInputs[i]->render(appearance, cAnd);
      mStep += cInputs[i]->getElementCount();
      glPopMatrix();
    }
    appearance->print(0.0f, -mYScale * 0.8f + mHeightStep * 0.5f, mYScale, IFont::Alignment::CENTER, cAnd ? "&" : "|");

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

  int PropertyCondition::FunctionBlock::getElementCount() const {
    int mElementCount = 0;
    for (int i = 0; i < static_cast<int>(cInputs.size()); i++) {
      mElementCount += cInputs[i]->getElementCount();
    }
    return mElementCount;
  }

  void PropertyCondition::FunctionBlock::confirm() {
    if (cParent->cSelectedNotGate) {
      cNot = !cNot;
    } else {
      cParent->showConditionPalette(cAnd);
    }
  }

  bool PropertyCondition::FunctionBlock::input(ConfiguratorSignalID id) {
    switch (id) {
      case ConfiguratorSignalID::CONFIRM: confirm(); return true;
      default:                                       break;
    }
    return false;
  }

  PropertyCondition::IFunctionOutput* PropertyCondition::FunctionBlock::getOutput() const {
    return cOutput;
  }

  int PropertyCondition::FunctionBlock::getIndex(IFunctionInput* input) const {
    for (int i = 0; i < static_cast<int>(cInputs.size()); i++) {
      if (input == cInputs[i].get()) {
        return i;
      }
    }
    throw ArgumentException("ERROR: PropertyCondition::FunctionBlock::getIndex: Specified function input not found.");
  }

  PropertyCondition::IFunctionInput* PropertyCondition::FunctionBlock::getAsInput() {
    return this;
  }

  PropertyCondition::IFunctionInput* PropertyCondition::FunctionBlock::getInput(int index) const {
    return cInputs[std::clamp(index, 0, static_cast<int>(cInputs.size() - 1))].get();
  }

  bool PropertyCondition::FunctionBlock::setAnd(bool isAnd) {
    cAnd = isAnd;
    return true;
  }

  std::optional<Condition> PropertyCondition::FunctionBlock::createCondition() {
    std::optional<Condition> mCondition = Condition(cAnd, cNot);
    for (std::unique_ptr<IFunctionInput>& mInput : cInputs) {
      std::optional<Condition> mSubCondition = mInput->createCondition();
      if (mSubCondition.has_value()) {
        mCondition->add(mSubCondition.value());
      }
    }
    return mCondition;
  }

  float PropertyCondition::FunctionBlock::getSelectedHeight() const {
    return (cParent->cSelectedNotGate ? 0.0f : (getElementCount() - cInputs[0]->getElementCount()) * 0.5f) + cOutput->getSelectedHeight(this);
  }

  int PropertyCondition::FunctionBlock::getMaxDepth() const {
    int mMaxDepth = 0;
    for (const std::unique_ptr<IFunctionInput>& mInput : cInputs) {
      mMaxDepth = std::max(mMaxDepth, mInput->getMaxDepth());
    }
    return mMaxDepth + 1;
  }

  bool PropertyCondition::FunctionBlock::isAnd() const {
    return cAnd;
  }

  float PropertyCondition::FunctionBlock::getSelectedHeight(const IFunctionInput* input) const {
    int mStep = 0;
    for (int i = static_cast<int>(cInputs.size()) - 1; i >= 0; i--) {
      if (cInputs[i].get() == input) {
        return mStep + cOutput->getSelectedHeight(this);
      }
      mStep += cInputs[i]->getElementCount();
    }
    throw ArgumentException("ERROR: PropertyCondition::FunctionBlock::getSelectedHeight: Specified function input not found.");
  }

  void PropertyCondition::FunctionBlock::setInput(int index, std::unique_ptr<IFunctionInput> input) {
    cInputs[index] = std::move(input);
    if (index == static_cast<int>(cInputs.size()) - 1) {
      addInput(std::make_unique<ElementInput>(cParent, nullptr, this));
    }
  }

  PropertyCondition::IFunctionInput* PropertyCondition::FunctionBlock::removeInput(int index) {
    if (index < static_cast<int>(cInputs.size() - 1)) {
      cInputs.erase(cInputs.begin() + index);
    }
    return cInputs[index].get();
  }

  PropertyCondition::ElementInput::ElementInput(PropertyCondition* parent, ConditionElement::Clause* input, IFunctionOutput* output) :
            cParent(parent),
            cInput(input),
            cOutput(output) {
  }

  void PropertyCondition::ElementInput::render(IPropertyAppearance* appearance, bool tick) const {
    float mYScale = 0.05f;
    float mXScale = mYScale;
    float mXSpacing = 0.2f;

    if (this != cParent->cSelectedInput || !cParent->cShowingConditionPalette) {
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

  int PropertyCondition::ElementInput::getElementCount() const {
    return 1;
  }

  void PropertyCondition::ElementInput::confirm() {
    if (cParent->cSelectedNotGate) {
      if (cInput != nullptr) {
        cInput = cInput->getNegatedClause();
      }
    } else {
      cParent->showConditionPalette(cInput != nullptr ? cInput->getElement() : nullptr);
    }
  }

  bool PropertyCondition::ElementInput::input(ConfiguratorSignalID id) {
    switch (id) {
      case ConfiguratorSignalID::CONFIRM: confirm(); return true;
      default:                                       break;
    }
    return false;
  }

  PropertyCondition::IFunctionOutput* PropertyCondition::ElementInput::getOutput() const {
    return cOutput;
  }

  PropertyCondition::IFunctionInput* PropertyCondition::ElementInput::getInput(int index) const {
    return nullptr;
  }

  bool PropertyCondition::ElementInput::setAnd(bool isAnd) {
    return false;
  }

  std::optional<Condition> PropertyCondition::ElementInput::createCondition() {
    if (cInput != nullptr) {
      std::optional<Condition> mCondition = Condition(false, false);
      mCondition->add(cInput);
      return mCondition;
    }
    return std::nullopt;
  }

  float PropertyCondition::ElementInput::getSelectedHeight() const {
    return cOutput != nullptr ? cOutput->getSelectedHeight(this) : 0.0f;
  }

  int PropertyCondition::ElementInput::getMaxDepth() const {
    return 1;
  }

  PropertyCondition::ResultOutput::ResultOutput(PropertyCondition* parent) :
            cParent(parent) {
  }

  void PropertyCondition::ResultOutput::setInput(std::unique_ptr<IFunctionInput> input) {
    cInput = std::move(input);
  }

  void PropertyCondition::ResultOutput::render(IPropertyAppearance* appearance) {
    glPushMatrix();
    float mYScale = 0.05f;
    float mXScale = mYScale;
    float mXSpacing = 0.2f;
    float mYSpacing = mYScale * 2.5f;

    glTranslatef((mXSpacing + mXScale * 2.0f) * cParent->cSelectionDepth.animation(), -cParent->cSelectionHeight.animation() * mYSpacing, 0.0f);
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
    cInput->render(appearance, true);
    glPopMatrix();
  }

  int PropertyCondition::ResultOutput::getMaxDepth() const {
    return cInput->getMaxDepth();
  }

  int PropertyCondition::ResultOutput::getElementCount() const {
    return cInput->getElementCount();
  }

  std::optional<Condition> PropertyCondition::ResultOutput::createCondition() {
    return cInput->createCondition();
  }

  int PropertyCondition::ResultOutput::getIndex(IFunctionInput* input) const {
    return 0;
  }

  PropertyCondition::IFunctionInput* PropertyCondition::ResultOutput::getInput(int index) const {
    return cInput.get();
  }

  void PropertyCondition::ResultOutput::setInput(int index, std::unique_ptr<IFunctionInput> input) {
    cInput = std::move(input);
  }

  PropertyCondition::IFunctionInput* PropertyCondition::ResultOutput::removeInput(int index) {
    cInput = std::make_unique<ElementInput>(cParent, nullptr, this);
    return cInput.get();
  }

  PropertyCondition::IFunctionInput* PropertyCondition::ResultOutput::getAsInput() {
    return nullptr;
  }

  float PropertyCondition::ResultOutput::getSelectedHeight(const IFunctionInput* input) const {
    return 0.0f;
  }

  bool PropertyCondition::ResultOutput::isAnd() const {
    return true;
  }
}
