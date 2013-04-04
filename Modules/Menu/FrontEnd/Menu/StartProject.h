#ifndef START_PROJECT_H
#define START_PROJECT_H

#include <IsoRealms/ICommand.h>

#include "../../IController.h"

class StartProject:public ICommand {
  private:
  IController* cController;
  std::string cProject;
  
  public:
  StartProject(IController*, const std::string&);
    
  void execute();
};

#endif
