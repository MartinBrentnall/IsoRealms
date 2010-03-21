#ifndef ELEMENT_H
#define ELEMENT_H

#include "IElement.h"
#include "IElementFactory.h"

/**
 * 
 */
template<class T1 = IElementSet, class T2 = IElementFactory> class Element:public IElement {
  private:

  /**
   * TODO:
   */
  T2* cElementFactory;

  /**
   * The element container is the entity that is holding the element for use in
   * game.  For example, this may be a Zone or a Map.
   */
  IElementContainer* cElementContainer;

  public:
  Element(T2* elementFactory) {
    cElementFactory = elementFactory;
    cElementContainer = NULL;
  }

  /***********************\
   * Implements IElement *
  \***********************/
  T1* getElementSet() {
    return cElementFactory->getElementSet();
  }

  T2* getElementFactory() {
    return cElementFactory;
  }

  void signalElementDirty() {
    if (cElementContainer) {
      cElementContainer->elementDirty(this);
    }
  }

  void setElementContainer(IElementContainer* elementContainer) {
    cElementContainer = elementContainer;
  }

  std::vector<IDynamicElement*> getDynamicElements() {
    std::vector<IDynamicElement*> mEmptyVector;
    return mEmptyVector;
  }

  std::vector<IDynamicElement*> getDynamicElementsRuntime() {
    std::vector<IDynamicElement*> mEmptyVector;
    return mEmptyVector;
  }

  void save(DOMNodeWriter*, BlockLocation&) {}
  bool initElement() {return true;}
  void removed() {}
  void added() {}
  void setRuntimeContext(IMap*) {}
  void renderStatic() {}
  void renderStaticEditing() {}

  ~Element() {}
};

#endif
