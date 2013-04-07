#ifndef ELEMENT_HANDLER_H
#define ELEMENT_HANDLER_H

#include <GL/gl.h>
#include <vector>

#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IVisualElement.h>

#include "IElement.h"

class ElementHandler {
  private:
  bool cUpdateStatic;
  GLuint cDisplayList;
  GLuint cEditingDisplayList;
  std::vector<IElement*> cElements;
  std::vector<IElement*> cDynamicElementsRuntime;
  std::vector<IElement*> cDynamicElementsEditing;
  std::vector<IElement*> cVisualElementsRuntime;
  std::vector<IElement*> cVisualElementsEditing;
  std::vector<IElement*> cDirtyElements;
  
  int getIndex(IElement*);
  
  void updateStatic();

  public:
  ElementHandler();
    
  void addElement(IElement*);
  void removeElement(IElement*);
  void setAllDirty();
  bool contains(IElement*);
  void setDirty(IElement*);
  bool isDirty();
  void updateEditing(unsigned int);
  void updateRuntime(unsigned int);
  bool init(unsigned int, bool);
  void renderStatic();
  void renderRuntime();
  void renderEditing();
  void staticChanged();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
};

#endif
