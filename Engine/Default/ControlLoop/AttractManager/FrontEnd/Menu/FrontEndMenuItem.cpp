#include "FrontEndMenuItem.h"

FrontEndMenuItem::FrontEndMenuItem(IFrontEndCommands* commandRegistry, DOMNodeWrapper* node) {
  cMenuText = node->getAttribute("name");
  cCommand = commandRegistry->parseCommand(node);
}

FrontEndMenuItem::FrontEndMenuItem(const std::string& name, ICommand* command) {
  cMenuText = name;
  cCommand = command;
}

void FrontEndMenuItem::render(int i, float fade, IFont* font, bool selected) {
  glColor4f(0.0f, 0.0f, 0.0f, fade);
  font->print(0.008f, (-0.10f * i) - 0.008f, 0.05f, 1, cMenuText.c_str());
  if (selected) {
    glColor4f(0.6f, 1.0f, 0.4f, fade);
  } else {
    glColor4f(1.0f, 1.0f, 1.0f, fade);
  }
  font->print(0.0f, -0.10f * i, 0.05f, 1, cMenuText.c_str());
}

FocusAction FrontEndMenuItem::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_UP: {
      return PREVIOUS;
    }

    case SDLK_DOWN: {
      return NEXT;
    }

    case SDLK_RETURN: {
      if (cCommand != NULL) {
        cCommand->execute();
      } else {
        std::cout << "Warning: No command set for menu item " << cMenuText << std::endl;
      }
      break;
    }

    default: {
      // Nothing to do.
    }
  }
  return NOTHING;
}
