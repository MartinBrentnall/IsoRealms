/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "FrontEndMenu.h"

FrontEndMenu::FrontEndMenu(IFrontEndCommands* commandRegistry, IMenuStack* menuStack, DOMNodeWrapper* node, const std::string& menuName, std::vector<std::string> tree) {
  cMenuName = menuName;
  tree.push_back(cMenuName);
  cSelectedItem = 0;

  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      FrontEndMenuItem* mMenuItem = new FrontEndMenuItem(commandRegistry, mNode);
      cMenuItems.push_back(mMenuItem);
    } else if (mValueAsString == "Menu") {
      std::string mName = mNode->getAttribute("name");
      FrontEndMenu* mSubMenu = new FrontEndMenu(commandRegistry, menuStack, mNode, mName, tree);
      ICommand* mEnterMenuCommand = new EnterMenuCommand(menuStack, mSubMenu);
      FrontEndMenuItem* mMenuItem = new FrontEndMenuItem(mName, mEnterMenuCommand);
      cMenuItems.push_back(mMenuItem);
    } else if (mValueAsString == "MenuList") {
      std::string mDirForSelection = mNode->getAttribute("source");
      std::string mActualDir = System::getProgramResource(mDirForSelection);
      std::vector<std::string>* mFileList = System::getFileList(mActualDir); // TODO: Destruction
      for (unsigned int i = 0; i < mFileList->size(); i++) {
        std::size_t mExtensionPosition = (*mFileList)[i].find_last_of('.');
        std::string mProjectName = (*mFileList)[i].substr(0, mExtensionPosition);
        FrontEndMenu* mSubMenu = new FrontEndMenu(commandRegistry, menuStack, mNode, mProjectName, tree);
        ICommand* mEnterMenuCommand = new EnterMenuCommand(menuStack, mSubMenu);
        FrontEndMenuItem* mMenuItem = new FrontEndMenuItem(mProjectName, mEnterMenuCommand);
        cMenuItems.push_back(mMenuItem);
      }
    } else if (mValueAsString == "CommandList") {
      std::string mDirForSelection = mNode->getAttribute("source");
      std::string mActualDir = System::getProgramResource(mDirForSelection);
      std::vector<std::string>* mFileList = System::getFileList(mActualDir); // TODO: Destruction
      for (unsigned int i = 0; i < mFileList->size(); i++) {
        std::size_t mExtensionPosition = (*mFileList)[i].find_last_of('.');
        std::string mProjectName = (*mFileList)[i].substr(0, mExtensionPosition);
        ICommand* mArgumentedCommand = commandRegistry->parseArgumentCommand(mNode, (*mFileList)[i]);
        FrontEndMenuItem* mMenuItem = new FrontEndMenuItem(mProjectName, mArgumentedCommand);
        cMenuItems.push_back(mMenuItem);
      }
    } else if (mValueAsString == "ControlSetup") {
      std::string mFilename = format(mNode->getAttribute("name"), tree);
      std::vector<IFrontEndMenuItem*> mControlMenuItems = parseControlConfig(mFilename);
      cMenuItems.insert(cMenuItems.end(), mControlMenuItems.begin(), mControlMenuItems.end());
    } else if (mValueAsString == "FullScreen") {
      std::string mName = mNode->getAttribute("name");
      std::string mTrueValue = mNode->getAttribute("trueValue");
      std::string mFalseValue = mNode->getAttribute("falseValue");
      BooleanMenuItem* mBooleanMenuItem = new BooleanMenuItem(mName, mTrueValue, mFalseValue);
      cMenuItems.push_back(mBooleanMenuItem);
    } else if (mValueAsString == "Resolution") {
      std::string mName = mNode->getAttribute("name");
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
      std::vector<ScreenMode*> mScreenModes = mScreenConfiguration->getAvailableModes();
      ScreenMode* mCurrentScreenMode = mScreenConfiguration->getScreenMode();
      std::vector<IOption*> mResolutionOptions;
      int mSelectedMode = -1;
      for (unsigned int j = 0; j < mScreenModes.size(); j++) {
        if (*mCurrentScreenMode == *(mScreenModes[j])) {
          mSelectedMode = j;
        }
        mResolutionOptions.push_back(new ResolutionOption(mScreenConfiguration, mScreenModes[j]));
      }
      if (mSelectedMode == -1) {
        mResolutionOptions.push_back(new ResolutionOption(mScreenConfiguration, mCurrentScreenMode));
        mSelectedMode = mResolutionOptions.size() - 1;
      }
      OptionMenuItem* mOptionMenuItem = new OptionMenuItem(mName, mResolutionOptions, mSelectedMode);
      cMenuItems.push_back(mOptionMenuItem);
      cApplicableItems.push_back(mOptionMenuItem);
      menuStack->testOnExit(this, mOptionMenuItem);
    }
  }
}

bool FrontEndMenu::isNumeric(const std::string& format, std::size_t start, std::size_t end) {
  for (std::size_t i = start; i < end; i++) {
    if (format[i] < '0' || format[i] > '9') {
      return false;
    }
  }
  return true;
}

std::size_t FrontEndMenu::getFormatChars(const std::string& format, std::size_t position) {
  std::size_t mPosition = format.find_first_of('m', position);
  if (mPosition != std::string::npos) {
    if (isNumeric(format, position + 1, mPosition)) {
      return (mPosition - position) + 1;
    }
  }
  return 1;
}

std::string FrontEndMenu::format(const std::string& format, std::vector<std::string> tree) {
  std::size_t mPosition = 0;
  std::size_t mPreviousPosition;
  std::string mReplacement;
  while (mPosition != std::string::npos) {
    mPreviousPosition = mPosition;
    mPosition = format.find_first_of('%', mPosition);
    mReplacement += format.substr(mPreviousPosition, mPosition - mPreviousPosition);
    if (mPosition != std::string::npos) {
      std::size_t mFormatChars = getFormatChars(format, mPosition);
      std::string mFormatSection = format.substr(mPosition, mFormatChars);
      if (mFormatSection[mFormatSection.length() - 1] == 'm') {
        std::string mNumberSection = mFormatSection.substr(1, mFormatSection.length() - 2);
        int mValue = atoi(mNumberSection.c_str());
        mReplacement += tree[mValue];
      }
      mPosition += mFormatChars;
    }
  }
  return mReplacement;
}

std::vector<IFrontEndMenuItem*> FrontEndMenu::parseControlConfig(const std::string& name) {
  InputCommands* mInputCommands = new InputCommands(name);
  std::vector<IFrontEndMenuItem*> mConfigurationItems;
  std::vector<std::string> mInputNames = mInputCommands->getInputNames();
  for (unsigned int i = 0; i < mInputNames.size(); i++) {
    DigitalInput* mDigitalInput = mInputCommands->findDigitalInput(mInputNames[i]);
    DigitalInputMenuItem* mDigitalInputMenuItem = new DigitalInputMenuItem(mInputNames[i], mDigitalInput);
    mConfigurationItems.push_back(mDigitalInputMenuItem);
  }
  return mConfigurationItems;
}

bool FrontEndMenu::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      switch (cMenuItems[cSelectedItem]->keyDown(event.key.keysym.sym)) {
        case NEXT: {
          if (cSelectedItem < cMenuItems.size() - 1) {
            cSelectedItem++;
          }
          return true;
        }
        
        case PREVIOUS: {
          if (cSelectedItem > 0) {
            cSelectedItem--;
          }
          return true;
        }
        
        default: {
          // Nothing to do
        }
      }
    }
  }
  return false;
}

void FrontEndMenu::render(float fade, IFont* font) {
  glEnable(GL_BLEND);
  for (unsigned int i = 0; i < cMenuItems.size(); i++) {
    cMenuItems[i]->render(i, fade, font, cSelectedItem == i);
  }
  glDisable(GL_BLEND);
}

void FrontEndMenu::update(unsigned int ticks) {
  // Nothing to do
}

void FrontEndMenu::applyAll() {
  for (unsigned int i = 0; i < cApplicableItems.size(); i++) {
    cApplicableItems[i]->apply();
  }
}
