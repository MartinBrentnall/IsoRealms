#ifndef COMPONENT_FILE_SELECTOR_H
#define COMPONENT_FILE_SELECTOR_H

#include "../../System.h"
#include "../RectangularComponent.h"

#include "IFileSelectionListener.h"

class ComponentFileSelector:public RectangularComponent {
  private:
  std::vector<std::string> cDirectory;
  IFileSelectionListener* cListener;
  
  void refresh();
  
  public:
  ComponentFileSelector(IResourceAccessor*, const std::string&, IFileSelectionListener*);
  
  std::string getSelectedFile();
  void assertSelection(const std::string&, const std::string&);
};

#endif
