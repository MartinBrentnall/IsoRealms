#include "DigitalInputMenuItem.h"

DigitalInputMenuItem::DigitalInputMenuItem(const std::string& name, DigitalInput* input) {
  cName = name;
  cDigitalInput = input;
}

void DigitalInputMenuItem::render(int i, float fade, IFont* font, bool selected) {
  glColor4f(0.0f, 0.0f, 0.0f, fade);
  font->print(0.008f, (-0.10f * i) - 0.008f, 0.05f, 1, cName.c_str());
  if (selected) {
    glColor4f(0.6f, 1.0f, 0.4f, fade);
  } else {
    glColor4f(1.0f, 1.0f, 1.0f, fade);
  }
  font->print(0.0f, -0.10f * i, 0.05f, 1, cName.c_str());
}

FocusAction DigitalInputMenuItem::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_UP: {
      return PREVIOUS;
    }

    case SDLK_DOWN: {
      return NEXT;
    }

    case SDLK_RETURN: {
      std::cout << "TODO: Implement this!" << std::endl;
      break;
    }

    default: {
      // Nothing to do.
    }
  }
  return NOTHING;
}
