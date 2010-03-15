#ifndef RUNTIME_H
#define RUNTIME_H

#include <string>
#include <vector>

#include "../../../../Global/CommandManager.h"
#include "../../../../Global/DOMNodeWrapper.h"
#include "../../../../Global/ICommand.h"
#include "../../../../Global/Map.h"

#include "../../IControlLoop.h"

class Runtime:public IControlLoop {
  private:
  bool cRunExitCommands;
  std::vector<ICommand*> cExitCommands;
  Map* cMap;

  std::vector<ICommand*> parseCommands(DOMNodeWrapper* node);
  void keyDown(SDLKey&);

  public:
  Runtime(DOMNodeWrapper*);

  /***************************\
   * Implements IControlLoop *
  \***************************/
  void input(SDL_Event&);
  void execute(int);
};

#endif
