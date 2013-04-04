#include "StartProject.h"

StartProject::StartProject(IController* controller, const std::string& project) {
  cController = controller;
  cProject = project;
}

void StartProject::execute() {
  cController->startProject(cProject);
}
