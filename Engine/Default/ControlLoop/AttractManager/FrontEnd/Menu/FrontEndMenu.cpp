#include "FrontEndMenu.h"

FrontEndMenu::FrontEndMenu(IFrontEndCommands* commandRegistry, IMenuStack* menuStack, DOMNodeWrapper* node) {
  cMenuName = node->getAttribute("name");
  cSelectedItem = 0;

  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "MenuItem") {
      FrontEndMenuItem* mMenuItem = new FrontEndMenuItem(commandRegistry, mNode);
      cMenuItems.push_back(mMenuItem);
    } else if (mValueAsString == "Menu") {
      FrontEndMenu* mSubMenu = new FrontEndMenu(commandRegistry, menuStack, mNode);
      std::string mName = mNode->getAttribute("name");
      ICommand* mEnterMenuCommand = new EnterMenuCommand(menuStack, mSubMenu);
      FrontEndMenuItem* mMenuItem = new FrontEndMenuItem(mName, mEnterMenuCommand);
      cMenuItems.push_back(mMenuItem);
    } else if (mValueAsString == "MenuItemList") {
      FrontEndMenu* mSubMenu = new FrontEndMenu(commandRegistry, node);
      std::string mName = mNode->getAttribute("name");
      ICommand* mEnterMenuCommand = new EnterMenuCommand(menuStack, mSubMenu);
      FrontEndMenuItem* mMenuItem = new FrontEndMenuItem(mName, mEnterMenuCommand);
      cMenuItems.push_back(mMenuItem);
    }
  }
}

FrontEndMenu::FrontEndMenu(IFrontEndCommands* commandRegistry, DOMNodeWrapper* node) {
  cSelectedItem = 0;
  std::string mDirForSelection = node->getAttribute("fromdir");
  std::string mFileNameFilter = node->getAttribute("filter");
  std::string mActualDir = System::getResource(mDirForSelection);
  std::vector<std::string>* mFileList = System::getFileList(mActualDir, mFileNameFilter); // TODO: Destruction
  for (unsigned int i = 0; i < mFileList->size(); i++) {
    ICommand* mArgumentCommand = commandRegistry->parseArgumentCommand(node, (*mFileList)[i]);
    FrontEndMenuItem* mMenuItem = new FrontEndMenuItem((*mFileList)[i], mArgumentCommand);
    cMenuItems.push_back(mMenuItem);
  }
}

void FrontEndMenu::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_UP: {
      if (cSelectedItem > 0) {
        cSelectedItem--;
      }
      break;
    }

    case SDLK_DOWN: {
      if (cSelectedItem < cMenuItems.size() - 1) {
        cSelectedItem++;
      }
      break;
    }

    case SDLK_RETURN: {
      cMenuItems[cSelectedItem]->execute();
      break;
    }

    default: {
      // Nothing to do.
    }
  }
}

void FrontEndMenu::mouseButtonDown() {
  std::cout << "Mouse Button Down" << std::endl;
}

bool FrontEndMenu::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      keyDown(event.key.keysym.sym);
      return true;
    }

    case SDL_MOUSEBUTTONDOWN: {
      mouseButtonDown();
      return true;
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
