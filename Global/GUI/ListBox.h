#ifndef LIST_BOX_H
#define LIST_BOX_H

#include <GL/gl.h>
#include <string>
#include <vector>

#include "ISizedComponent.h"
#include "LookAndFeel.h"

class ListBox:public ISizedComponent {
  private:
  unsigned int cSelectedItem;
  std::vector<std::string> cItems;

  bool keyDown(SDLKey&);

  public:
  ListBox();

  std::string& getSelectedItem();
  void addItem(const std::string&);
  void removeItem(const std::string&);

  /******************************\
   * Implements ISizedComponent *
  \******************************/
  void update(int);
  void render();
  bool input(SDL_Event&);
  float getWidth();
  float getHeight();
};

#endif
