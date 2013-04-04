#ifndef DYNAMIC_ELEMENT_GROUP_H
#define DYNAMIC_ELEMENT_GROUP_H

#include <GL/gl.h>

#include <IsoRealms/Resources/ElementType/ElementHandler.h>
#include <IsoRealms/Resources/ElementType/IElementGroup.h>

class DynamicElementGroup:public IElementGroup,
                          public IInteractiveElement,
			  public IDynamicElement,
			  public IVisualElement {
  private:
  ElementHandler cElementHandler;
  bool cActive;
  float cVisibility;

  public:
  void setActive(bool);
  void addElement(IElement*);
  void removeElement(IElement*);
  void update(unsigned int);
  void render();
  bool input(SDL_Event&);
  
  IPlugin* getElementSet();
  IElementType* getElementType();
  bool initElement(unsigned int);
  void renderStatic();
  void renderStaticEditing();
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IDynamicElement*> getDynamicElementsRuntime();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
};

#endif
