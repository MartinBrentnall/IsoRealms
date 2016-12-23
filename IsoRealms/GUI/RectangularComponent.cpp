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
#include "RectangularComponent.h"

RectangularComponent::RectangularComponent() {
  cFocusedComponent = nullptr;
  cActivePopupMenu = nullptr;
}

RectangularComponent::RectangularComponent(const std::string& description, IResourceAccessor* resourceAccessor, IEditingContext* editingContext, IDialogValueListener* valueListener) {
  std::string mFullPath = System::getProgramResource(description) + ".dialog";
  DOMNodeWrapper* mFileNode = new DOMNodeWrapper(mFullPath);
  cValueListener = valueListener;
  loadComponent(mFileNode, resourceAccessor, editingContext);
}

RectangularComponent::RectangularComponent(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  loadComponent(node, resourceAccessor);
}

void RectangularComponent::loadComponent(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor, IEditingContext* editingContext) {
  bool read = false;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Component") {
      loadDialog(mNode, this, 0.0f, resourceAccessor, editingContext);
      read = true;
    }
  }
  if (!read) {
    std::cout << "WARNING: NOT READ!" << std::endl;
    exit(1);
  }
  cFocusedComponent = nullptr;
  cActivePopupMenu = nullptr;
}

ListBox* RectangularComponent::getListBox(const std::string& componentName) {
  std::map<std::string, ListBox*>::iterator i = cListBoxComponents.find(componentName);
  return i != cListBoxComponents.end() ? i->second : nullptr;
}

void RectangularComponent::clearListBox(const std::string& listBoxName) {
  ListBox* mListBox = getListBox(listBoxName);
  mListBox->clear();
}

void RectangularComponent::addListBoxItem(const std::string& listBoxName, const std::string& item) {
  ListBox* mListBox = getListBox(listBoxName);
  mListBox->addItem(item);
}

std::string RectangularComponent::getStringValue(const std::string& componentName) {
  std::map<std::string, IValueComponent<std::string>*>::iterator i = cValueComponentsString.find(componentName);
  if (i != cValueComponentsString.end()) {
    return i->second->getValue();
  } else {
    std::cout << "WARNING: Specified string value component isn't known in this dialog!" << std::endl;
    // TODO: Chuck
    exit(1);
  }
  return nullptr;
}

void RectangularComponent::setComponentAction(const std::string& componentName, ICommand* command) {
  std::map<std::string, Button*>::iterator i = cCommandableComponents.find(componentName);
  if (i != cCommandableComponents.end()) {
    i->second->setCommand(command);
  } else {
    for (std::map<IRectangularComponent*, MenuPopup*>::iterator i = cPopupMenus.begin(); i != cPopupMenus.end(); i++) {
      if (i->second->setComponentAction(componentName, command)) {
	return;
      }
    }
    std::cout << "WARNING: Specified commandable component isn't known in this dialog!" << std::endl;
    // TODO: Chuck
    exit(1);
  }
}

void RectangularComponent::setComponentPadding(const std::string& componentName, float padding) {
  cPaddedComponents[componentName]->setPadding(padding);
}

std::string RectangularComponent::getSelectedTab(const std::string& container) {
  return cTabbedContainers[container]->getSelectedTab();
}

std::string RectangularComponent::getValue(const std::string& component) {
  return cListBoxComponents[component]->getSelectedItem();
}

void RectangularComponent::setSelectedTab(const std::string& container, const std::string& tab) {
  cTabbedContainers[container]->setSelectedTab(tab);
}

void RectangularComponent::addComponentAction(const std::string& parent, const std::string& actionText, ICommand* command) {
  for (std::map<IRectangularComponent*, MenuPopup*>::iterator i = cPopupMenus.begin(); i != cPopupMenus.end(); i++) {
    if (i->second->addComponentAction(parent, actionText, command)) {
      return;
    }
  }
  std::cout << "WARNING: Specified command container isn't known in this dialog!" << std::endl;
  // TODO: Chuck
  exit(1);
}

void RectangularComponent::setComponentText(DOMNodeWrapper* node, ITextComponent* component) {
  std::string mComponentText = node->getAttribute("text");
  component->setText(mComponentText);
}

IComponentBoundsCalculator* RectangularComponent::getBoundsCalculator(DOMNodeWrapper* node, IRectangle* parent, float padding, ISizedComponent* component) {
  std::string mAlignment = node->getAttribute("align");
  std::vector<std::string> mAlignWords = Utils::splitWords(mAlignment);

  // TODO: Cache this relation for later!
  EdgeRelation* mInsideParent = new EdgeRelation(parent, EdgeRelation::INSIDE, padding);
  EdgeRelation* mLeftEdge   = nullptr;
  EdgeRelation* mRightEdge  = nullptr;
  EdgeRelation* mTopEdge    = nullptr;
  EdgeRelation* mBottomEdge = nullptr;
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
    std::vector<std::string> mLeftWords = Utils::splitWords(mLeft);
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
    std::vector<std::string> mRightWords = Utils::splitWords(mRight);
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
    std::vector<std::string> mTopWords = Utils::splitWords(mTop);
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
    std::vector<std::string> mBottomWords = Utils::splitWords(mBottom);
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

  if (component == nullptr) {
    if (mLeftEdge   == nullptr) {mLeftEdge   = mInsideParent;}
    if (mRightEdge  == nullptr) {mRightEdge  = mInsideParent;}
    if (mTopEdge    == nullptr) {mTopEdge    = mInsideParent;}
    if (mBottomEdge == nullptr) {mBottomEdge = mInsideParent;}
  }
  // TODO: Dealloc mInsideParent relation
  // TODO: Dealloc FlexibleGrid
  return new ComponentEdgeLayout(mTopEdge, mLeftEdge, mBottomEdge, mRightEdge, component);
}

ISizedComponent* RectangularComponent::loadSizedComponent(DOMNodeWrapper* node, IEditingContext* editingContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Label") {
      std::string mLabelText = mNode->getStringValue();
      return new TextLabelComponent(mLabelText);
    } else if (mValueAsString == "TextField") {
      std::string mName = mNode->getAttribute("name");
      TextFieldComponent* mTextField = new TextFieldComponent();
      cValueComponentsString[mName] = mTextField;
      return mTextField;
    } else if (mValueAsString == "Slider") {
      std::string mName = mNode->getAttribute("name");
      SliderComponent* mSlider = new SliderComponent();
      cValueComponentsFloat[mName] = mSlider;
      cSliders[mName] = mSlider;
      return mSlider;
    } else if (mValueAsString == "Panel") {
      std::string mName = mNode->getAttribute("name");
      PanelComponent* mPanel = new PanelComponent();
      cPanels[mName] = mPanel;
      return mPanel;
    } else if (mValueAsString == "Selectable") {
      std::string mName = mNode->getAttribute("name");
      SelectableComponent* mSelectableComponent = new SelectableComponent();
      cSelectableComponents[mName] = mSelectableComponent;
      return mSelectableComponent;
    } else if (mValueAsString == "Button") {
      std::string mName = mNode->getAttribute("name");
      std::string mText = mNode->getAttribute("text");
      Button* mButton = new Button(nullptr, nullptr, mText);
      cCommandableComponents[mName] = mButton;
      return mButton;
    } else if (mValueAsString == "ResourceModel") {
      std::string mName = mNode->getAttribute("name");
      ComponentResourceModel* mComponentResourceModel = new ComponentResourceModel(0.12f, editingContext->getResourceSelector());
      mComponentResourceModel->addValueListener(this);
      cValueComponentsResource3DModelType[mName] = mComponentResourceModel;
      return mComponentResourceModel;
    } else {
//      std::cout << "WARNING: Unknown sized component tag: \"" << mValueAsString << "\"" << std::endl;
      // TODO: Throw
    }
  }  
  std::cout << "WARNING: Sized component is not known!" << std::endl;
  return nullptr;
}

void RectangularComponent::loadEvenGridCells(DOMNodeWrapper* node, GridLayoutComponent* grid, IResourceAccessor* resourceAccessor) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Cell") {
      int mRow = mNode->getIntegerAttribute("row") - 1;
      int mColumn = mNode->getIntegerAttribute("column") - 1;
      IComponentBoundsCalculator* mCellRectangle = grid->getCellLayout(mColumn, mRow);
      std::cout << "Loading even cell " << mRow << "," << mColumn << std::endl;
      loadDialog(mNode, mCellRectangle, 0.0f, resourceAccessor, nullptr);
    } else {
//      std::cout << "WARNING: Unknown cell tag: \"" << mValueAsString << "\"" << std::endl;
      // TODO: Throw
    }
  }
}

void RectangularComponent::loadFlexibleGridCells(DOMNodeWrapper* node, FlexibleGridLayoutComponent* grid, IEditingContext* editingContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Cell") {
      int mRow = mNode->getIntegerAttribute("row") - 1;
      int mColumn = mNode->getIntegerAttribute("column") - 1;
      ISizedComponent* mCellComponent = loadSizedComponent(mNode, editingContext);
      cChildren.push_back(mCellComponent);
      cFocusedComponent = mCellComponent;
      grid->addComponent(mCellComponent, mColumn, mRow);
    } else {
//      std::cout << "WARNING: Unknown cell tag: \"" << mValueAsString << "\"" << std::endl;
      // TODO: Throw
    }
  }  
}

void RectangularComponent::loadTabbedContainer(DOMNodeWrapper* node, TabbedContainer* tabbedContainer, IResourceAccessor* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Tab") {
      std::string mTabName = mNode->getAttribute("name");
      std::string mTabText = mNode->getAttribute("text");
      RectangularComponent* mTabComponent = new RectangularComponent();
      cComponentContainers[mTabName] = mTabComponent;
      mTabComponent->loadDialog(mNode, mTabComponent, 0.02f, resources, nullptr, this);
      IComponentBoundsCalculator* mTabLayout = tabbedContainer->getTabLayout();
      mTabComponent->setBoundsCalculator(mTabLayout);
      tabbedContainer->addTab(mTabName, mTabText, mTabComponent);
    }
  }
}

void RectangularComponent::loadPopupMenu(IRectangularComponent* component, DOMNodeWrapper* node, IResourceAccessor* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "PopupMenu") {
      MenuPopup* mMenuPopup = new MenuPopup(mNode, this, 0.0f, 0.0f, nullptr, nullptr, resources);
      cPopupMenus[component] = mMenuPopup;
      return;
    }
  }  
}

void RectangularComponent::addBooleanValueComponent(const std::string& name, IValueComponent<bool>* component) {
  cValueComponentsBoolean[name] = component;
}

void RectangularComponent::addIntegerValueComponent(const std::string& name, IValueComponent<int>* component) {
  cValueComponentsInteger[name] = component;
}

void RectangularComponent::addStringValueComponent(const std::string& name, IValueComponent<std::string>* component) {
  cValueComponentsString[name] = component;
}

void RectangularComponent::loadDialog(DOMNodeWrapper* node, IRectangle* parent, float padding, IResourceAccessor* resourceAccessor, IEditingContext* editingContext, RectangularComponent* parentComponent) {
  cTopLevelComponent = parentComponent != nullptr ? parentComponent : this;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "EvenGrid") {
      int mRows = mNode->getIntegerAttribute("rows");
      int mColumns = mNode->getIntegerAttribute("columns");
      std::string mName = mNode->getAttribute("name");
      IComponentBoundsCalculator* mGridLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      GridLayoutComponent* mGridComponent = new GridLayoutComponent(mColumns, mRows, mGridLayout);
      loadEvenGridCells(mNode, mGridComponent, resourceAccessor);
    } else if (mValueAsString == "FlexibleGrid") {
      int mRows = mNode->getIntegerAttribute("rows");
      int mColumns = mNode->getIntegerAttribute("columns");
      std::string mName = mNode->getAttribute("name");
      FlexibleGridLayoutComponent* mGridComponent = new FlexibleGridLayoutComponent(mColumns, mRows);
      IComponentBoundsCalculator* mGridLayout = getBoundsCalculator(mNode, parent, padding, mGridComponent);
      mGridComponent->setBoundsCalculator(mGridLayout);
      loadFlexibleGridCells(mNode, mGridComponent, editingContext);
      cSizedComponents[mName] = mGridComponent;
    } else if (mValueAsString == "WrappingGrid") {
      std::string mName = mNode->getAttribute("name");
      ScrollableContainer* mGridContainer = new ScrollableContainer();
      WrappingGridComponent* mGridComponent = new WrappingGridComponent(0.02f);
      mGridContainer->setRootComponent(mGridComponent);
      IComponentBoundsCalculator* mGridLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      mGridContainer->setBoundsCalculator(mGridLayout);
      cComponentContainers[mName] = mGridComponent;
      cPaddedComponents[mName] = mGridComponent;
      cChildren.push_back(mGridContainer);
      cFocusedComponent = mGridContainer;
      loadPopupMenu(mGridContainer, mNode, resourceAccessor);
    } else if (mValueAsString == "TextEditor") {
      std::string mName = mNode->getAttribute("name");
      std::string mText = mNode->getAttribute("text");
      ScrollableContainer* mTextEditorContainer = new ScrollableContainer();
      TextEditorComponent* mTextEditorComponent = new TextEditorComponent(mText);
      mTextEditorContainer->setRootComponent(mTextEditorComponent);
      IComponentBoundsCalculator* mTextEditorLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      mTextEditorContainer->setBoundsCalculator(mTextEditorLayout);
      cChildren.push_back(mTextEditorContainer);
      cTopLevelComponent->addStringValueComponent(mName, mTextEditorComponent);
      cFocusedComponent = mTextEditorContainer;
    } else if (mValueAsString == "Button") {
      std::string mText = mNode->getAttribute("text");
      std::string mName = mNode->getAttribute("name");
      Button* mButton = new Button(nullptr, nullptr, mText);
      IComponentBoundsCalculator* mButtonLayout = getBoundsCalculator(mNode, parent, padding, mButton);
      mButton->setBoundsCalculator(mButtonLayout);
      cSizedComponents[mName] = mButton;
      cCommandableComponents[mName] = mButton;
      cChildren.push_back(mButton);
      cFocusedComponent = mButton;
    } else if (mValueAsString == "CheckBox") {
      std::string mLabelText = mNode->getStringValue();
      std::string mName = mNode->getAttribute("name");
      CheckBox* mCheckBox = new CheckBox(mLabelText);
      IComponentBoundsCalculator* mLabelLayout = getBoundsCalculator(mNode, parent, padding, mCheckBox);
      mCheckBox->setBoundsCalculator(mLabelLayout);
      cSizedComponents[mName] = mCheckBox;
      cTopLevelComponent->addBooleanValueComponent(mName, mCheckBox);
      cChildren.push_back(mCheckBox);
      cFocusedComponent = mCheckBox;
    } else if (mValueAsString == "Label") {
      std::string mLabelText = mNode->getStringValue();
      std::string mName = mNode->getAttribute("name");
      TextLabelComponent* mLabel = new TextLabelComponent(mLabelText);
      IComponentBoundsCalculator* mLabelLayout = getBoundsCalculator(mNode, parent, padding, mLabel);
      mLabel->setBoundsCalculator(mLabelLayout);
      cSizedComponents[mName] = mLabel;
      cChildren.push_back(mLabel);
      cFocusedComponent = mLabel;
    } else if (mValueAsString == "IntegerField") {
      std::string mName = mNode->getAttribute("name");
      IntegerField* mIntegerField = new IntegerField();
      IComponentBoundsCalculator* mIntegerFieldLayout = getBoundsCalculator(mNode, parent, padding, mIntegerField);
      mIntegerField->setBoundsCalculator(mIntegerFieldLayout);
      cSizedComponents[mName] = mIntegerField;
      cTopLevelComponent->addIntegerValueComponent(mName, mIntegerField);
      cChildren.push_back(mIntegerField);
      cFocusedComponent = mIntegerField;
    } else if (mValueAsString == "TextField") {
      std::string mName = mNode->getAttribute("name");
      TextFieldComponent* mTextField = new TextFieldComponent();
      IComponentBoundsCalculator* mTextFieldLayout = getBoundsCalculator(mNode, parent, padding, mTextField);
      mTextField->setBoundsCalculator(mTextFieldLayout);
      cSizedComponents[mName] = mTextField;
      cTopLevelComponent->addStringValueComponent(mName, mTextField);
      cChildren.push_back(mTextField);
      cFocusedComponent = mTextField;
    } else if (mValueAsString == "Slider") {
      std::string mName = mNode->getAttribute("name");
      SliderComponent* mSlider = new SliderComponent();
      IComponentBoundsCalculator* mSliderLayout = getBoundsCalculator(mNode, parent, padding, mSlider);
      mSlider->setBoundsCalculator(mSliderLayout);
      cSizedComponents[mName] = mSlider;
      cSliders[mName] = mSlider;
      cValueComponentsFloat[mName] = mSlider;
      cChildren.push_back(mSlider);
      cFocusedComponent = mSlider;
    } else if (mValueAsString == "Panel") {
      std::string mName = mNode->getAttribute("name");
      PanelComponent* mPanel = new PanelComponent();
      IComponentBoundsCalculator* mPanelLayout = getBoundsCalculator(mNode, parent, padding, mPanel);
      mPanel->setBoundsCalculator(mPanelLayout);
      cSizedComponents[mName] = mPanel;
      cPanels[mName] = mPanel;
      cChildren.push_back(mPanel);
      cFocusedComponent = mPanel;
    } else if (mValueAsString == "ListBox") {
      std::string mName = mNode->getAttribute("name");
      ScrollableContainer* mListContainer = new ScrollableContainer();
      ListBox* mListBox = new ListBox();
      mListBox->addListener(this);
      mListContainer->setRootComponent(mListBox);
      IComponentBoundsCalculator* mListLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      mListContainer->setBoundsCalculator(mListLayout);
      cListBoxComponents[mName] = mListBox;
      cChildren.push_back(mListContainer);
      cFocusedComponent = mListContainer;
    } else if (mValueAsString == "ScrollableContainer") {
      std::string mName = mNode->getAttribute("name");
      ScrollableContainer* mScrollableContainer = new ScrollableContainer();
      IComponentBoundsCalculator* mLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      mScrollableContainer->setBoundsCalculator(mLayout);
      cComponentContainers[mName] = mScrollableContainer;
      cChildren.push_back(mScrollableContainer);
      cFocusedComponent = mScrollableContainer;
    } else if (mValueAsString == "Container") {
      std::string mName = mNode->getAttribute("name");
      Container* mContainer = new Container();
      IComponentBoundsCalculator* mLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      mContainer->setBoundsCalculator(mLayout);
      cComponentContainers[mName] = mContainer;
      cChildren.push_back(mContainer);
      cFocusedComponent = mContainer;
    } else if (mValueAsString == "TabbedContainer") {
      std::string mName = mNode->getAttribute("name");
      TabbedContainer* mTabbedContainer = new TabbedContainer();
      IComponentBoundsCalculator* mTabbedContainerLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      mTabbedContainer->setBoundsCalculator(mTabbedContainerLayout);
      loadTabbedContainer(mNode, mTabbedContainer, resourceAccessor);
      cTabbedContainers[mName] = mTabbedContainer;
      cChildren.push_back(mTabbedContainer);
      cFocusedComponent = mTabbedContainer;
    } else if (mValueAsString == "Selectable") {
      std::string mName = mNode->getAttribute("name");
      SelectableComponent* mSelectableComponent = new SelectableComponent();
      IComponentBoundsCalculator* mSelectableComponentLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      mSelectableComponent->setBoundsCalculator(mSelectableComponentLayout);
      cSizedComponents[mName] = mSelectableComponent;
      cSelectableComponents[mName] = mSelectableComponent;
      cChildren.push_back(mSelectableComponent);
      cFocusedComponent = mSelectableComponent;
    } else if (mValueAsString == "ResourceModel") {
      std::string mName = mNode->getAttribute("name");
      ComponentResourceModel* mComponentResourceModel = new ComponentResourceModel(0.12f, editingContext->getResourceSelector());
      IComponentBoundsCalculator* mComponentResourceModelLayout = getBoundsCalculator(mNode, parent, padding, nullptr);
      mComponentResourceModel->setBoundsCalculator(mComponentResourceModelLayout);
      cValueComponentsResource3DModelType[mName] = mComponentResourceModel;
      cSizedComponents[mName] = mComponentResourceModel;
      cChildren.push_back(mComponentResourceModel);
      cFocusedComponent = mComponentResourceModel;
    } else {
//      std::cout << "WARNING: Unknown dialog tag: \"" << mValueAsString << "\"" << std::endl;
//      exit(1);
      // TODO: Throw
    }
  }
}

void RectangularComponent::update(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cChildren.size(); i++) {
    cChildren[i]->update(milliseconds);
  }
  if (cActivePopupMenu != nullptr) {
// TODO    cActivePopupMenu->update(milliseconds);
  }
  updateContent(milliseconds);
}

void RectangularComponent::render() {
//   glEnable(GL_SCISSOR_TEST);
//   int mLeftPixels = mScreen->convertToXPixels(mLeft);
//   int mRightPixels = mScreen->convertToXPixels(mRight);
//   int mTopPixels = mScreen->convertToYPixels(mTop);
//   int mBottomPixels = mScreen->convertToYPixels(mBottom);
//   glScissor(mLeftPixels + 1, mBottomPixels + 1, (mRightPixels - mLeftPixels) - 1, (mTopPixels - mBottomPixels) - 1);

  renderContent();
  for (unsigned int i = 0; i < cChildren.size(); i++) {
    cChildren[i]->render();
  }
  if (cActivePopupMenu != nullptr) {
    cActivePopupMenu->render();
  }
//   glDisable(GL_SCISSOR_TEST);
}

void RectangularComponent::testFocusChange(SDL_Event& event) {
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.button.x);
    float mY = mScreen->getYLocation(event.button.y);
    if (contains(mX, mY)) {
      for (unsigned int i = 0; i < cChildren.size(); i++) {
        if (cChildren[i]->contains(mX, mY)) {
          if (cFocusedComponent != nullptr) {
            cFocusedComponent->lostFocus();
          }
          cFocusedComponent = cChildren[i];
          cFocusedComponent->gainedFocus();
          cActivePopupMenu = nullptr;
          if (event.button.button == SDL_BUTTON_RIGHT) {
            std::map<IRectangularComponent*, MenuPopup*>::iterator i = cPopupMenus.find(cFocusedComponent);
            if (i != cPopupMenus.end()) {
              cActivePopupMenu = i->second;
              cActivePopupMenu->setPosition(mX, mY);
            }
          }
          return;
        }
      }
    }
    if (cFocusedComponent != nullptr) {
      cFocusedComponent->lostFocus();
    }
    cFocusedComponent = nullptr;
    cActivePopupMenu = nullptr;
  }
}

bool RectangularComponent::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    if (cFocusedComponent != nullptr && cFocusedComponent->input(event)) {
      return true;
    }
  }
  return false;
}

bool RectangularComponent::input(SDL_Event& event) {
  if (cActivePopupMenu != nullptr) {
    if (cActivePopupMenu->input(event)) {
      return true;
    }
  }
  testFocusChange(event);
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (mouseButtonDown(event)) {
        return true;
      }
      break;
    }

    case SDL_MOUSEBUTTONUP: {
      if (cFocusedComponent != nullptr && cFocusedComponent->input(event)) {
        return true;
      }
      break;
    }

    case SDL_MOUSEMOTION: {
      if (cFocusedComponent != nullptr && cFocusedComponent->input(event)) {
        return true;
      }
      break;
    }

    default: {
      if (cFocusedComponent != nullptr && cFocusedComponent->input(event)) {
        return true;
      }
      break;
    }
  }
  return inputContent(event);
}

void RectangularComponent::setRenderer(ISliderRenderer* renderer, const std::string& name) {
  cSliders[name]->setRenderer(renderer);
}

void RectangularComponent::setRenderer(IPanelRenderer* renderer, const std::string& name) {
  cPanels[name]->setRenderer(renderer);
}

void RectangularComponent::setSelectable(const std::string& name, ISelector* selectable) {
  cSelectableComponents[name]->setHandler(selectable);
}

void RectangularComponent::addFloatListener(IValueListener<float>* listener, const std::string& name) {
  cValueComponentsFloat[name]->addValueListener(listener);
}

void RectangularComponent::addStringListener(IValueListener<std::string>* listener, const std::string& name) {
  cValueComponentsString[name]->addValueListener(listener);
}

void RectangularComponent::addBooleanListener(IValueListener<bool>* listener, const std::string& name) {
  cValueComponentsBoolean[name]->addValueListener(listener);
}

void RectangularComponent::addIntegerListener(IValueListener<int>* listener, const std::string& name) {
  cValueComponentsInteger[name]->addValueListener(listener);
}

void RectangularComponent::setBooleanValue(const std::string& name, bool value) {
  cValueComponentsBoolean[name]->setValue(value);
}

void RectangularComponent::setIntegerValue(const std::string& name, int value) {
  cValueComponentsInteger[name]->setValue(value);
}

void RectangularComponent::setFloatValue(const std::string& name, float value) {
  cValueComponentsFloat[name]->setValue(value);
}

void RectangularComponent::setStringValue(const std::string& name, const std::string& value) {
  for (std::map<std::string, IValueComponent<std::string>*>::iterator i = cValueComponentsString.begin(); i != cValueComponentsString.end(); i++) {
    std::cout << "String value component: " << i->first << std::endl;
  }
  cValueComponentsString[name]->setValue(value);
}

void RectangularComponent::setStringValue(const std::string& name, float value) {
  cValueComponentsString[name]->setValue(Utils::toString(value));
}

void RectangularComponent::setValue(const std::string& name, I3DModelType* value) {
  cValueComponentsResource3DModelType[name]->setValue(value);
}

void RectangularComponent::addComponent(IRectangularComponent* component) {
  cChildren.push_back(component);
}

void RectangularComponent::addComponent(const std::string& container, ISizedComponent* component) {
  std::string mTopContainer = container;
  std::string mSubContainer;
  std::string::size_type mSplit = container.find('/');
  if (mSplit != std::string::npos) {
    mTopContainer = container.substr(0, mSplit);
    mSubContainer = container.substr(mSplit + 1);
  }
  IComponentHolder* mComponentHolder = cComponentContainers[mTopContainer];
  mComponentHolder->addComponent(mSubContainer, component);
//  cComponentContainers[mTopContainer]->addComponent(mSubContainer, component);
}

void RectangularComponent::removeComponent(ISizedComponent* component) {
  for (std::map<std::string, IComponentHolder*>::iterator i = cComponentContainers.begin(); i != cComponentContainers.end(); i++) {
    i->second->removeComponent(component);
  }
}

void RectangularComponent::setFocusedComponent(IRectangularComponent* component) {
  cFocusedComponent = component;
}

float RectangularComponent::getWidth() {
  float mLeft = FLT_MAX;
  float mRight = -FLT_MAX;
  for (unsigned int i = 0; i < cChildren.size(); i++) {
    mLeft = min(mLeft, cChildren[i]->getLeft());
    mRight = max(mRight, cChildren[i]->getRight());
  }
  return mRight - mLeft;
}

float RectangularComponent::getHeight() {
  float mBottom = FLT_MAX;
  float mTop = -FLT_MAX;
  for (unsigned int i = 0; i < cChildren.size(); i++) {
    mBottom = min(mBottom, cChildren[i]->getBottom());
    mTop = max(mTop, cChildren[i]->getTop());
  }
  return mTop - mBottom;
}

void RectangularComponent::closeMenu(IMenu* menu) {
  cActivePopupMenu = nullptr;
}

void RectangularComponent::assertSelection(ListBox* listBox, const std::string& item) {
  for (std::map<std::string, ListBox*>::iterator i = cListBoxComponents.begin(); i != cListBoxComponents.end(); i++) {
    if (listBox == i->second) {
      assertSelection(i->first, item);
    }
  }
}

void RectangularComponent::valueChanged(IValueComponent<I3DModelType*>* component, I3DModelType* value) {
  for (std::map<std::string, IValueComponent<I3DModelType*>*>::iterator i = cValueComponentsResource3DModelType.begin(); i != cValueComponentsResource3DModelType.end(); i++) {
    if (component == i->second) {
      cValueListener->valueChanged(i->first, value);
      return;
    }
  }
}
