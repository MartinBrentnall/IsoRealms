/*
 * Copyright 2009 Martin Brentnall
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
#include "ListSelectionBox.h"

IFont* ListSelectionBox::cFont = NULL;

void ListSelectionBox::setFont(IFont* font) {
  cFont = font;
}

ListSelectionBox::ListSelectionBox(IComponentContainer* componentContainer, IItemSelectedCommand* listener, std::vector<IListItem*>& items, float x, float y) : RectangleComponent(componentContainer, x, y, 1.0f, 1.0f) {
  if (listener == NULL) {
    // TODO: Throw exception
    std::cout << "WARNING: Application will crash on selection!" << std::endl;
  }
  cSelectedItem = 0;
  cListener = listener;
  // TODO: Implement this!
}

ListSelectionBox::ListSelectionBox(IComponentContainer* componentContainer, IItemSelectedCommand* listener, std::vector<std::string>& items, float x, float y) : RectangleComponent(componentContainer, x, y, 1.0f, 1.0f) {
  if (listener == NULL) {
    // TODO: Throw exception
    std::cout << "WARNING: Application will crash on selection!" << std::endl;
  }
  cSelectedItem = 0;
  cListener = listener;
  cItems = &items;
}

void ListSelectionBox::updateContent(int milliseconds) {
  // TODO
}

void ListSelectionBox::renderContent() {
  float mLine = (getY() + getHeight()) - 0.05f;
  for (unsigned int i = 0; i < cItems->size(); i++) {
    if (i == cSelectedItem) {
      glColor3f(0.0f, 1.0f, 0.0f);
    } else {
      glColor3f(1.0f, 1.0f, 1.0f);
    }
    cFont->print(getX() + 0.02f, mLine, 0.02f, 0, (*cItems)[i].c_str());
    mLine -= 0.05f;
  }
}

bool ListSelectionBox::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_DOWN: {
      if (cSelectedItem < cItems->size() - 1) {
        cSelectedItem++;
      }
      return true;
    }

    case SDLK_UP: {
      if (cSelectedItem > 0) {
        cSelectedItem--;
      }
      return true;
    }

    case SDLK_RETURN: {
      cListener->itemSelected((*cItems)[cSelectedItem]);
      close();
      return true;
    }

    case SDLK_ESCAPE: {
      close();
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool ListSelectionBox::inputContent(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}
