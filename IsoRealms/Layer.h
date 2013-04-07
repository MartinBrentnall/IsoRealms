#ifndef LAYER_H
#define LAYER_H

#include "Persistence/DOMNodeWrapper.h"
#include "Resources/Resources.h"

class Layer:public IElementContainer {
  private:
  IElement* cRootElement;
  
  public:
  Layer(DOMNodeWrapper*, Resources*);
  
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void renderRuntime();
  void renderEditing();
  void input(SDL_Event&);
  void initRuntime();
  void save(DOMNodeWriter*, IResourceLocator*);
  void staticChanged();
  
  /*********************\
   * IElementContainer *
  \*********************/
  void addElement(IElement*);
  void removeElement(IElement*);
};

#endif
