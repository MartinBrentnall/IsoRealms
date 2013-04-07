#ifndef DYNAMIC_ELEMENT_GROUP_H
#define DYNAMIC_ELEMENT_GROUP_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <IsoRealms/Resources/ElementType/ElementHandler.h>
#include <IsoRealms/Resources/ElementType/IElementGroup.h>
#include <IsoRealms/Resources/IResourceAccessor.h>

class DynamicElementGroup:public IElementGroup {
  private:
  ElementHandler cElementHandler;
  bool cActive;
  float cVisibility;

  public:
  void setActive(bool);
  void addElement(IElement*);
  void removeElement(IElement*);
  
  IPlugin* getElementSet();
  IElementType* getElementType();
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  bool initElement(unsigned int);
  void renderStatic();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();

  void renderRuntime();
  void renderEditing();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void input(SDL_Event&);
  bool isVisualRuntime();
  bool isVisualEditing();
  bool isDynamicRuntime();
  bool isDynamicEditing();
  bool isInteractive();  
};

#endif
