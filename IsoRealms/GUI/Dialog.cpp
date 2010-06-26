/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "Dialog.h"

const float Dialog::TITLE_BAR_HEIGHT = 0.06f;

Dialog::Dialog(IComponentContainer* componentContainer, const std::string& dialogDescriptionFile) {
  cComponentContainer = componentContainer;
  cX = 0.0f;
  cY = 0.0f;
  cWidth = 0.75f;
  cHeight = 0.75;
  cDragging = false;
  cFocusedComponent = NULL;

  std::string mFullPath = System::getResource(dialogDescriptionFile) + ".dialog";
  DOMNodeWrapper* mFileNode = new DOMNodeWrapper(mFullPath);
  for (int i = 0; i < mFileNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mFileNode->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Dialog") {
      cTitle = mNode->getAttribute("title");
      loadDialog(mNode, this, 0.02f);
    }
  }
}

void Dialog::setComponentText(DOMNodeWrapper* node, ITextComponent* component) {
  std::string mComponentText = node->getAttribute("text");
  component->setText(mComponentText);
}

// TODO: Move this into a generic "split into words" function class... or something
std::vector<std::string> Dialog::splitWords(std::string& words) {
  std::vector<std::string> mSplitWords;
  std::string::size_type mWordStart = words.find_first_not_of(' ');
  while (mWordStart != std::string::npos) {
    words = words.substr(mWordStart);
    std::string::size_type mWordEnd = words.find_first_of(' ');
    if (mWordEnd != std::string::npos) {
      std::string mAlignWord = words.substr(0, mWordEnd);
      words = words.substr(mWordEnd);
      mSplitWords.push_back(mAlignWord);
      mWordStart = words.find_first_not_of(' ');
    } else {
      mSplitWords.push_back(words);
      mWordStart = std::string::npos;
    }
  }
  return mSplitWords;
}

IComponentBoundsCalculator* Dialog::getBoundsCalculator(DOMNodeWrapper* node, IRectangle* parent, float padding, ISizedComponent* component) {
  std::string mAlignment = node->getAttribute("align");
  std::vector<std::string> mAlignWords = splitWords(mAlignment);

  // TODO: Cache this relation for later!
  EdgeRelation* mInsideParent = new EdgeRelation(parent, EdgeRelation::INSIDE, padding);
  EdgeRelation* mLeftEdge   = NULL;
  EdgeRelation* mRightEdge  = NULL;
  EdgeRelation* mTopEdge    = NULL;
  EdgeRelation* mBottomEdge = NULL;
  for (unsigned int i = 0; i < mAlignWords.size(); i++) {
    if      (mAlignWords[i] == "left")   {mLeftEdge   = mInsideParent;}
    else if (mAlignWords[i] == "right")  {mRightEdge  = mInsideParent;}
    else if (mAlignWords[i] == "top")    {mTopEdge    = mInsideParent;}
    else if (mAlignWords[i] == "bottom") {mBottomEdge = mInsideParent;}
    else {
      std::cout << "WARNING: Unknown word in alignment attribute: \"" << mAlignWords[i] << "\"" << std::endl;
    }
  }

  // TODO: Warn about ignored things in case parent is fully used and relations are still specified

  std::string mLeft = node->getAttribute("left");
  std::string mRight = node->getAttribute("right");
  if (mLeft != "") {
    std::vector<std::string> mLeftWords = splitWords(mLeft);
    for (unsigned int i = 0; i < mLeftWords.size(); i++) {
      // Find component to go to the right of
      std::map<std::string, ISizedComponent*>::iterator j = cSizedComponents.find(mLeftWords[i]);
      if (j != cSizedComponents.end()) {
        // TODO: Cache this relation for later!
        // TODO: Dealloc this relation!
        mLeftEdge = new EdgeRelation(j->second, EdgeRelation::OUTSIDE);
        break;
      }
    }
  }
  if (mRight != "") {
    std::vector<std::string> mRightWords = splitWords(mRight);
    for (unsigned int i = 0; i < mRightWords.size(); i++) {
      // Find component to go to the left of
      std::map<std::string, ISizedComponent*>::iterator j = cSizedComponents.find(mRightWords[i]);
      if (j != cSizedComponents.end()) {
        // TODO: Cache this relation for later!
        // TODO: Dealloc this relation!
        mRightEdge = new EdgeRelation(j->second, EdgeRelation::OUTSIDE);
        break;
      }
    }
  }

  std::string mTop = node->getAttribute("top");
  std::string mBottom = node->getAttribute("bottom");
  if (mTop != "") {
    std::vector<std::string> mTopWords = splitWords(mTop);
    for (unsigned int i = 0; i < mTopWords.size(); i++) {
      // Find component to go below
      std::map<std::string, ISizedComponent*>::iterator j = cSizedComponents.find(mTopWords[i]);
      if (j != cSizedComponents.end()) {
        // TODO: Cache this relation for later!
        // TODO: Dealloc this relation!
        mTopEdge = new EdgeRelation(j->second, EdgeRelation::OUTSIDE);
        break;
      }
    }
  }
  if (mBottom != "") {
    std::vector<std::string> mBottomWords = splitWords(mBottom);
    for (unsigned int i = 0; i < mBottomWords.size(); i++) {
      // Find component to go below
      std::map<std::string, ISizedComponent*>::iterator j = cSizedComponents.find(mBottomWords[i]);
      if (j != cSizedComponents.end()) {
        // TODO: Cache this relation for later!
        // TODO: Dealloc this relation!
        mBottomEdge = new EdgeRelation(j->second, EdgeRelation::OUTSIDE);
        break;
      }
    }
  }

  if (component == NULL) {
    if (mLeftEdge == NULL)   {mLeftEdge   = mInsideParent;}
    if (mRightEdge == NULL)  {mRightEdge  = mInsideParent;}
    if (mTopEdge == NULL)    {mTopEdge    = mInsideParent;}
    if (mBottomEdge == NULL) {mBottomEdge = mInsideParent;}
  }
  // TODO: Dealloc mInsideParent relation
  // TODO: Dealloc FlexibleGrid
  return new ComponentEdgeLayout(mTopEdge, mLeftEdge, mBottomEdge, mRightEdge, component);
}

ISizedComponent* Dialog::loadSizedComponent(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Label") {
      std::string mLabelText = mNode->getStringValue();
      return new TextLabelComponent(mLabelText);
    } else if (mValueAsString == "TextField") {
      std::string mName = mNode->getAttribute("name");
      TextFieldComponent* mTextField = new TextFieldComponent();
      cStringValueComponents[mName] = mTextField;
      return mTextField;
    } else {
      std::cout << "WARNING: Unknown sized component tag: \"" << mValueAsString << "\"" << std::endl;
      // TODO: Throw
    }
  }  
  return NULL;
}

void Dialog::loadEvenGridCells(DOMNodeWrapper* node, GridLayoutComponent* grid) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Cell") {
      int mRow = mNode->getIntegerAttribute("row") - 1;
      int mColumn = mNode->getIntegerAttribute("column") - 1;
      IComponentBoundsCalculator* mCellRectangle = grid->getCellLayout(mColumn, mRow);
      std::cout << "Loading even cell " << mRow << "," << mColumn << std::endl;
      loadDialog(mNode, mCellRectangle, 0.0f);
    } else {
      std::cout << "WARNING: Unknown cell tag: \"" << mValueAsString << "\"" << std::endl;
      // TODO: Throw
    }
  }
}

void Dialog::loadFlexibleGridCells(DOMNodeWrapper* node, FlexibleGridLayoutComponent* grid) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Cell") {
      int mRow = mNode->getIntegerAttribute("row") - 1;
      int mColumn = mNode->getIntegerAttribute("column") - 1;
      ISizedComponent* mCellComponent = loadSizedComponent(mNode);
      addComponent(mCellComponent);
      setFocusedComponent(mCellComponent);
      grid->addComponent(mCellComponent, mColumn, mRow);
    } else {
      std::cout << "WARNING: Unknown cell tag: \"" << mValueAsString << "\"" << std::endl;
      // TODO: Throw
    }
  }  
}

void Dialog::loadDialog(DOMNodeWrapper* node, IRectangle* parent, float padding) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "EvenGrid") {
      int mRows = mNode->getIntegerAttribute("rows");
      int mColumns = mNode->getIntegerAttribute("columns");
      std::string mName = mNode->getAttribute("name");
      IComponentBoundsCalculator* mGridLayout = getBoundsCalculator(mNode, parent, padding, NULL);
      GridLayoutComponent* mGridComponent = new GridLayoutComponent(mColumns, mRows, mGridLayout);
      loadEvenGridCells(mNode, mGridComponent);
    } else if (mValueAsString == "FlexibleGrid") {
      int mRows = mNode->getIntegerAttribute("rows");
      int mColumns = mNode->getIntegerAttribute("columns");
      std::string mName = mNode->getAttribute("name");
      FlexibleGridLayoutComponent* mGridComponent = new FlexibleGridLayoutComponent(mColumns, mRows);
      IComponentBoundsCalculator* mGridLayout = getBoundsCalculator(mNode, parent, padding, mGridComponent);
      mGridComponent->setBoundsCalculator(mGridLayout);
      loadFlexibleGridCells(mNode, mGridComponent);
      cSizedComponents[mName] = mGridComponent;
    } else if (mValueAsString == "Button") {
      std::string mText = mNode->getAttribute("text");
      std::string mName = mNode->getAttribute("name");
      Button* mButton = new Button(NULL, NULL, mText);
      IComponentBoundsCalculator* mButtonLayout = getBoundsCalculator(mNode, parent, padding, mButton);
      mButton->setBoundsCalculator(mButtonLayout);
      cSizedComponents[mName] = mButton;
      cCommandableComponents[mName] = mButton;
      addComponent(mButton);
      setFocusedComponent(mButton);
    } else if (mValueAsString == "Label") {
      std::string mLabelText = mNode->getStringValue();
      std::string mName = mNode->getAttribute("name");
      TextLabelComponent* mLabel = new TextLabelComponent(mLabelText);
      IComponentBoundsCalculator* mLabelLayout = getBoundsCalculator(mNode, parent, padding, mLabel);
      mLabel->setBoundsCalculator(mLabelLayout);
      cSizedComponents[mName] = mLabel;
      addComponent(mLabel);
      setFocusedComponent(mLabel);
    } else if (mValueAsString == "TextField") {
      std::string mName = mNode->getAttribute("name");
      TextFieldComponent* mTextField = new TextFieldComponent();
      IComponentBoundsCalculator* mTextFieldLayout = getBoundsCalculator(mNode, parent, padding, mTextField);
      mTextField->setBoundsCalculator(mTextFieldLayout);
      cSizedComponents[mName] = mTextField;
      cStringValueComponents[mName] = mTextField;
      addComponent(mTextField);
      setFocusedComponent(mTextField);
    } else if (mValueAsString == "ListBox") {
      std::string mName = mNode->getAttribute("name");
      ScrollableContainer* mListContainer = new ScrollableContainer();
      ListBox* mListBox = new ListBox();
      mListContainer->setRootComponent(mListBox);
      IComponentBoundsCalculator* mListLayout = getBoundsCalculator(mNode, parent, padding, NULL);
      mListContainer->setBoundsCalculator(mListLayout);
      cListBoxComponents[mName] = mListBox;
      addComponent(mListContainer);
      setFocusedComponent(mListContainer);
    } else {
      std::cout << "WARNING: Unknown dialog tag: \"" << mValueAsString << "\"" << std::endl;
      // TODO: Throw
    }
  }
}

Dialog::Dialog(IComponentContainer* componentContainer, const std::string& title, float x, float y, float width, float height) {
  cComponentContainer = componentContainer;
  cTitle = title;
  cX = x;
  cY = y;
  cWidth = width;
  cHeight = height;
  cDragging = false;
  cFocusedComponent = NULL;
}

void Dialog::setComponentAction(const std::string& componentName, ICommand* command) {
  std::map<std::string, Button*>::iterator i = cCommandableComponents.find(componentName);
  if (i != cCommandableComponents.end()) {
    i->second->setCommand(command);
  } else {
    std::cout << "WARNING: Specified commandable component isn't known in this dialog!" << std::endl;
    // TODO: Chuck
    exit(1);
  }
}

ListBox* Dialog::getListBox(const std::string& componentName) {
  std::map<std::string, ListBox*>::iterator i = cListBoxComponents.find(componentName);
  return i != cListBoxComponents.end() ? i->second : NULL;
}

std::string Dialog::getStringValue(const std::string& componentName) {
  std::map<std::string, TextFieldComponent*>::iterator i = cStringValueComponents.find(componentName);
  if (i != cStringValueComponents.end()) {
    return i->second->getText();
  } else {
    std::cout << "WARNING: Specified commandable component isn't known in this dialog!" << std::endl;
    // TODO: Chuck
    exit(1);
  }
  return NULL;
}

float Dialog::getLeft() {
  return cX;
}

float Dialog::getBottom() {
  return cY;
}

float Dialog::getRight() {
  return cX + cWidth;
}

float Dialog::getTop() {
  return cY + cHeight;
}

bool Dialog::contains(float x, float y) {
  return x >= cX && x <= cX + cWidth && y >= cY && y <= cY + cHeight + TITLE_BAR_HEIGHT;
}

void Dialog::translate(float x, float y) {
  cX += x;
  cY += y;
}

void Dialog::resize(float x, float y) {
  cWidth += x;
  cHeight += y;
}

void Dialog::update(int milliseconds) {
  for (unsigned int i = 0; i < cChildren.size(); i++) {
    cChildren[i]->update(milliseconds);
  }
  updateContent(milliseconds);
}

void Dialog::render() {
  float mLeft = getLeft();
  float mTop = getTop();
  float mRight = getRight();
  float mBottom = getBottom();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);

  glColor4f(0.85f, 0.0f, 0.7f, 0.85f); glVertex2f(mRight, mTop + TITLE_BAR_HEIGHT);
                                       glVertex2f(mLeft, mTop + TITLE_BAR_HEIGHT);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f); glVertex2f(mLeft, mTop);
                                       glVertex2f(mRight, mTop);
  glEnd();
  glDisable(GL_BLEND);

  glBegin(GL_LINE_LOOP);
  glColor3f(0.8f, 0.6f, 1.0f);
  glVertex2f(mLeft,  mTop + TITLE_BAR_HEIGHT);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop + TITLE_BAR_HEIGHT);
  glEnd();

  glEnable(GL_SCISSOR_TEST);
  int mLeftPixels = mScreen->convertToXPixels(mLeft);
  int mRightPixels = mScreen->convertToXPixels(mRight);
  int mTopPixels = mScreen->convertToYPixels(mTop + TITLE_BAR_HEIGHT);
  int mBottomPixels = mScreen->convertToYPixels(mBottom);
  glScissor(mLeftPixels + 1, mBottomPixels + 1, (mRightPixels - mLeftPixels) - 1, (mTopPixels - mBottomPixels) - 1);

  // Close button
  glEnd();
  float mButtonPadding = TITLE_BAR_HEIGHT * 0.25f;
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f((mRight - mAspectRatio * TITLE_BAR_HEIGHT) + mAspectRatio * mButtonPadding,  mTop + mButtonPadding);
  glVertex2f( mRight - mAspectRatio * mButtonPadding,                                    (mTop + TITLE_BAR_HEIGHT) - mButtonPadding);
  glVertex2f((mRight - mAspectRatio * TITLE_BAR_HEIGHT) + mAspectRatio * mButtonPadding, (mTop + TITLE_BAR_HEIGHT) - mButtonPadding);
  glVertex2f( mRight - mAspectRatio * mButtonPadding,                                     mTop + mButtonPadding);
  glEnd();
  glLineWidth(1.0f);

  // Title bar text
  IFont* mFont = LookAndFeel::getDefaultFont();
  float mFontSize = LookAndFeel::getDefaultFontSize();
  mFont->print(mLeft + 0.01f, mTop + 0.01f, mFontSize, 0, cTitle.c_str());
  
  for (unsigned int i = 0; i < cChildren.size(); i++) {
    cChildren[i]->render();
  }
  renderContent();
  glDisable(GL_SCISSOR_TEST);

  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

bool Dialog::mouseButtonDown(SDL_Event& event) {
  float mTop = getTop();
  float mRight = getRight();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    if (cFocusedComponent != NULL && cFocusedComponent->input(event)) {
      return true;
    }
  }
  if (mX >= mRight - mAspectRatio * TITLE_BAR_HEIGHT && mX <= mRight && mY >= mTop && mY <= mTop + TITLE_BAR_HEIGHT) {
    close();
    return true;
  }
  if (contains(mX, mY)) {
    cComponentContainer->bringComponentToFront(this);
    cDragging = true;
    return true;
  }
  return false;
}

bool Dialog::mouseMotion(SDL_Event& event) {
  if (cDragging) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.motion.xrel) + 1.0f;
    float mY = mScreen->getYLocation(event.motion.yrel) - 1.0f;
    translate(mX, mY);
    return true;
  }
  return false;
}

void Dialog::relinquishFocus() {
  cComponentContainer->relinquishFocus(this);
}

void Dialog::close() {
  for (unsigned int i = 0; i < cCloseListeners.size(); i++) {
    cCloseListeners[i]->componentClosed(this);
  }
  cComponentContainer->removeComponent(this);
}

void Dialog::addCloseListener(IComponentCloseListener* listener) {
  cCloseListeners.push_back(listener);
}

void Dialog::addComponent(IHUDComponent* child) {
  cChildren.push_back(child);
}

void Dialog::setFocusedComponent(IHUDComponent* child) {
  cFocusedComponent = child;
  // TODO: Make sure focused component actually exists in this dialog.
}

void Dialog::testFocusChange(SDL_Event& event) {
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.button.x);
    float mY = mScreen->getYLocation(event.button.y);
    if (contains(mX, mY)) {
      for (unsigned int i = 0; i < cChildren.size(); i++) {
        if (cChildren[i]->contains(mX, mY)) {
          cFocusedComponent = cChildren[i];
          return;
        }
      }
    }
  }
}

bool Dialog::input(SDL_Event& event) {
  testFocusChange(event);
  if (inputContent(event)) {
    return true;
  }
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }

    case SDL_MOUSEBUTTONUP: {
      cDragging = false;
      if (cFocusedComponent != NULL && cFocusedComponent->input(event)) {
        return true;
      }
      return false;
    }

    case SDL_MOUSEMOTION: {
      if (cFocusedComponent != NULL && cFocusedComponent->input(event)) {
        return true;
      }
      return mouseMotion(event);
    }

    default: {
      if (cFocusedComponent != NULL && cFocusedComponent->input(event)) {
        return true;
      }
    }
  }
  return false;
}

Dialog::~Dialog() {
  for (unsigned int i = 0; i < cChildren.size(); i++) {
    delete cChildren[i];
  }
}
